#include "../header/job_scheduler.h"

// initializes the job scheduler with the number of open threads
struct job_scheduler *initialize_job_scheduler(int number_of_threads) {
    error_handler(number_of_threads < 1,"number of threads is less than 0");

    struct job_scheduler *new_job_scheduler = (struct job_scheduler *)malloc(sizeof(struct job_scheduler));
    new_job_scheduler->thread_pool = malloc(sizeof(pthread_t)*number_of_threads);
    error_handler(new_job_scheduler->thread_pool == NULL,"malloc failed");

    new_job_scheduler->number_of_threads = number_of_threads;
    new_job_scheduler->jobs = 0;
    new_job_scheduler->stop = false;
    new_job_scheduler->pause = false;
    pthread_mutex_init(&new_job_scheduler->queue_mutex, NULL);
    pthread_cond_init(&new_job_scheduler->queue_empty, NULL);
    pthread_cond_init(&new_job_scheduler->queue_not_empty, NULL);
    pthread_cond_init(&new_job_scheduler->barrier, NULL);
    pthread_mutex_init(&new_job_scheduler->pause_mutex, NULL);
    pthread_cond_init(&new_job_scheduler->resume, NULL);
    new_job_scheduler->queue = initialize_queue();
    create_threads_job_scheduler(new_job_scheduler);

    return new_job_scheduler;
}

// creates all the threads in thread pool
void create_threads_job_scheduler(struct job_scheduler *job_scheduler) {
    error_handler(job_scheduler == NULL,"job scheduler is NULL");
    int return_value;
    for( int i = 0 ; i < job_scheduler->number_of_threads ; i++ ) {
        return_value = pthread_create(&(job_scheduler->thread_pool[i]),0,thread_function,(void *) job_scheduler);
        error_handler(return_value != 0,"pthread_create failed");
    }
}

// waits until executed all jobs in the queue
void barrier_job_scheduler(struct job_scheduler *job_scheduler) {
    error_handler(job_scheduler == NULL,"job scheduler is NULL");
    pthread_mutex_lock(&job_scheduler->queue_mutex);
    while( job_scheduler->jobs > 0 )
        pthread_cond_wait(&job_scheduler->queue_empty,&job_scheduler->queue_mutex);
    pthread_mutex_unlock(&job_scheduler->queue_mutex);
}

// executes jobs and waits until executed all jobs from a specific barrier in the queue
void dynamic_barrier_job_scheduler(struct job_scheduler *job_scheduler, int *barrier) {
    error_handler(job_scheduler == NULL,"job scheduler is NULL");
    while( (*barrier) != 0 ) {
        pthread_mutex_lock(&job_scheduler->queue_mutex);
        if( job_scheduler->queue->length != 0 )
            execute_job(job_scheduler);
        else if( (job_scheduler->queue->length == 0) && ((*barrier) != 0) ) {
            while( (job_scheduler->queue->length == 0) && ((*barrier) != 0) )
                pthread_cond_wait(&job_scheduler->barrier,&job_scheduler->queue_mutex);
            pthread_mutex_unlock(&job_scheduler->queue_mutex);
        }
        else if( (job_scheduler->queue->length == 0) && ((*barrier) == 0) )
            pthread_mutex_unlock(&job_scheduler->queue_mutex);
    }
}

// free all resources that are allocated by job scheduler
void free_job_scheduler(struct job_scheduler *job_scheduler) {
    if( job_scheduler == NULL )
        return;
    if( job_scheduler->thread_pool != NULL )
        free(job_scheduler->thread_pool);
    pthread_mutex_destroy(&job_scheduler->queue_mutex);
    pthread_cond_destroy(&job_scheduler->queue_empty);
    pthread_cond_destroy(&job_scheduler->queue_not_empty);
    pthread_cond_destroy(&job_scheduler->barrier);
    pthread_mutex_destroy(&job_scheduler->pause_mutex);
    pthread_cond_destroy(&job_scheduler->resume);
    free_queue(&job_scheduler->queue);
    free(job_scheduler);
}

// waits until all threads finish their job and after that close all threads
void stop_job_scheduler(struct job_scheduler *job_scheduler) {
    error_handler(job_scheduler == NULL,"job scheduler is NULL");
    job_scheduler->stop = true;
    pthread_cond_broadcast(&job_scheduler->queue_not_empty);
    for( int i = 0 ; i < job_scheduler->number_of_threads ; i++ )
        pthread_join(job_scheduler->thread_pool[i],0);
}

// adds a job in the queue
void schedule_job_scheduler(struct job_scheduler *job_scheduler, void (*function)(void*), void *argument, int *barrier) {
    error_handler(job_scheduler == NULL,"job scheduler is NULL");
    pthread_mutex_lock(&job_scheduler->queue_mutex);
    push_queue(&job_scheduler->queue, function, argument, barrier);
    job_scheduler->jobs++;
    pthread_cond_signal(&job_scheduler->queue_not_empty);
    pthread_cond_signal(&job_scheduler->barrier);
    pthread_mutex_unlock(&job_scheduler->queue_mutex);
}

// pops a job from the queue and executes it
void execute_job(struct job_scheduler *job_scheduler) {
    struct job *job = NULL;
    void (*function)(void*), *argument;

    error_handler(job_scheduler == NULL,"job scheduler is NULL");

    job = pop_queue(&job_scheduler->queue);
    pthread_mutex_unlock(&job_scheduler->queue_mutex);

    function = job->function;
    argument = job->argument;
    // execute the function
    function(argument);

    pthread_mutex_lock(&job_scheduler->queue_mutex);
    (*job->barrier)--;
    if( (*job->barrier) == 0 )
        pthread_cond_signal(&job_scheduler->barrier);
    job_scheduler->jobs--;
    if( job_scheduler->jobs == 0 )
        pthread_cond_signal(&job_scheduler->queue_empty);
    free_job(&job);
    pthread_mutex_unlock(&job_scheduler->queue_mutex);
}

// the new threads start execution by invoking this function
void *thread_function(void *job_scheduler_argument) {
    struct job_scheduler *job_scheduler = job_scheduler_argument;

    error_handler(job_scheduler == NULL,"job scheduler is NULL");
    
    signal(ORIGINAL_SIGNAL, (void (*))hold_threads);
    hold_threads(FAKE_SIGNAL, job_scheduler);

    while( true ) {
        pthread_mutex_lock(&job_scheduler->queue_mutex);
        while( (job_scheduler->queue->length == 0) && (job_scheduler->stop == false) )
            pthread_cond_wait(&job_scheduler->queue_not_empty,&job_scheduler->queue_mutex);
        if( job_scheduler->stop == true ) {
            pthread_mutex_unlock(&job_scheduler->queue_mutex);
            pthread_exit(0);
        }
        else
            execute_job(job_scheduler);
    }
}

// pause signal handler, holds threads until they get resumed
void hold_threads(const int signal, void *job_scheduler_argument) {
    static struct job_scheduler *job_scheduler = NULL;
    if( job_scheduler == NULL )
        job_scheduler = job_scheduler_argument;
    error_handler(job_scheduler == NULL,"job scheduler is NULL");
    if( signal == ORIGINAL_SIGNAL ) {
        pthread_mutex_lock(&job_scheduler->pause_mutex);
        while( job_scheduler->pause == true )
            pthread_cond_wait(&job_scheduler->resume,&job_scheduler->pause_mutex);
        pthread_mutex_unlock(&job_scheduler->pause_mutex);
    }
}

// sends a signal to all threads in thread pool and they fell asleep util they get resumed
void pause_job_scheduler(struct job_scheduler *job_scheduler) {
    error_handler(job_scheduler == NULL,"job scheduler is NULL");
    pthread_mutex_lock(&job_scheduler->pause_mutex);
    job_scheduler->pause = true;
    pthread_mutex_unlock(&job_scheduler->pause_mutex);
    for( int i = 0 ; i < job_scheduler->number_of_threads ; i++ )
        pthread_kill(job_scheduler->thread_pool[i], ORIGINAL_SIGNAL);
}

// resumes all threads in thread pool
void resume_job_scheduler(struct job_scheduler *job_scheduler) {
    error_handler(job_scheduler == NULL,"job scheduler is NULL");
    pthread_mutex_lock(&job_scheduler->pause_mutex);
    job_scheduler->pause = false;
    pthread_cond_broadcast(&job_scheduler->resume);
    pthread_mutex_unlock(&job_scheduler->pause_mutex);
}