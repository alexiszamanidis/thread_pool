#include "../header/job_scheduler.h"

// initializes the job with a function and argument
struct job *initialize_job(void (*function)(void*), void *argument, int *barrier) {
    if( function == NULL )
        return NULL;

    struct job *new_job = my_malloc(struct job,1);
    error_handler(new_job == NULL,"malloc failed");

    (*barrier)++;
    new_job->function = function;
    new_job->argument = argument;
    new_job->barrier = barrier;
    new_job->next = NULL;
    
    return new_job;
}

// frees a job and decreases the barrier
void free_job(struct job **job) {
    if( *job == NULL )
        return;
    (*(*job)->barrier)--;
    if( (*job)->argument != NULL )
        free_pointer(&((*job)->argument));
    free_pointer(job);
}

// initializes the job scheduler with the number of open threads
struct job_scheduler *initialize_job_scheduler(int number_of_threads) {
    error_handler(number_of_threads < 1,"number of threads is less than 1");
    int return_value;
    struct job_scheduler *new_job_scheduler = my_malloc(struct job_scheduler,1);
    error_handler(new_job_scheduler == NULL,"malloc failed");
    new_job_scheduler->thread_pool = my_malloc(pthread_t,number_of_threads);
    error_handler(new_job_scheduler->thread_pool == NULL,"malloc failed");

    new_job_scheduler->number_of_threads = number_of_threads;
    new_job_scheduler->jobs = 0;
    new_job_scheduler->stop = false;
    new_job_scheduler->pause = false;
    error_handler(pthread_mutex_init(&new_job_scheduler->queue_mutex, NULL) != 0,"pthread_mutex_init failed");
    error_handler(pthread_cond_init(&new_job_scheduler->queue_empty, NULL) != 0,"pthread_cond_init failed");
    error_handler(pthread_cond_init(&new_job_scheduler->queue_not_empty, NULL) != 0,"pthread_cond_init failed");
    error_handler(pthread_cond_init(&new_job_scheduler->barrier, NULL) != 0,"pthread_cond_init failed");
    error_handler(pthread_mutex_init(&new_job_scheduler->pause_mutex, NULL) != 0,"pthread_mutex_init failed");
    error_handler(pthread_cond_init(&new_job_scheduler->resume, NULL) != 0,"pthread_cond_init failed");
    new_job_scheduler->queue = initialize_queue();

#if THREAD_POOL_DEBUG
    printnl(new_job_scheduler->number_of_threads);
#endif

    for( int i = 0 ; i < new_job_scheduler->number_of_threads ; i++ ) {
        return_value = pthread_create(&(new_job_scheduler->thread_pool[i]),0,thread_function,(void *) new_job_scheduler);
        error_handler(return_value != 0,"pthread_create failed");
    }

    return new_job_scheduler;
}

// waits until executed all jobs in the queue
void barrier_job_scheduler(struct job_scheduler *job_scheduler) {
    error_handler(job_scheduler == NULL,"job scheduler is NULL");
    error_handler(pthread_mutex_lock(&job_scheduler->queue_mutex) != 0,"pthread_mutex_lock failed");
    while( job_scheduler->jobs > 0 )
        pthread_cond_wait(&job_scheduler->queue_empty,&job_scheduler->queue_mutex);
    error_handler(pthread_mutex_unlock(&job_scheduler->queue_mutex) != 0,"pthread_mutex_unlock failed");
}

// executes jobs and waits until executed all jobs from a specific barrier in the queue
void dynamic_barrier_job_scheduler(struct job_scheduler *job_scheduler, int *barrier) {
    error_handler(job_scheduler == NULL,"job scheduler is NULL");
    while( true ) {
        error_handler(pthread_mutex_lock(&job_scheduler->queue_mutex) != 0,"pthread_mutex_lock failed");
        if( (job_scheduler->queue->length != 0) && ((*barrier) != 0) )
            execute_job(job_scheduler);
        else if( (*barrier) == 0 ) {
            error_handler(pthread_mutex_unlock(&job_scheduler->queue_mutex) != 0,"pthread_mutex_unlock failed");
            break;
        }
        #define LOOP
        #ifdef LOOP
        else
            error_handler(pthread_mutex_unlock(&job_scheduler->queue_mutex) != 0,"pthread_mutex_unlock failed");
        #endif
        #ifdef CONDITION_VARIABLE
        else if( (job_scheduler->queue->length == 0) && ((*barrier) != 0) ) {
            while( (job_scheduler->queue->length == 0) && ((*barrier) != 0) )
                pthread_cond_wait(&job_scheduler->barrier,&job_scheduler->queue_mutex);
            error_handler(pthread_mutex_unlock(&job_scheduler->queue_mutex) != 0,"pthread_mutex_unlock failed");
        }
        #endif
    }
}

// free all resources that are allocated by job scheduler
void free_job_scheduler(struct job_scheduler *job_scheduler) {
    if( job_scheduler == NULL )
        return;

    stop_job_scheduler(job_scheduler);

    if( job_scheduler->thread_pool != NULL )
        free_pointer(&(job_scheduler->thread_pool));
    pthread_mutex_destroy(&job_scheduler->queue_mutex);
    pthread_cond_destroy(&job_scheduler->queue_empty);
    pthread_cond_destroy(&job_scheduler->queue_not_empty);
    pthread_cond_destroy(&job_scheduler->barrier);
    pthread_mutex_destroy(&job_scheduler->pause_mutex);
    pthread_cond_destroy(&job_scheduler->resume);
    free_queue(&job_scheduler->queue);
    free_pointer(&job_scheduler);
}

// waits until all threads finish their job and after that close all threads
void stop_job_scheduler(struct job_scheduler *job_scheduler) {
    error_handler(job_scheduler == NULL,"job scheduler is NULL");
    error_handler(pthread_mutex_lock(&job_scheduler->queue_mutex) != 0,"pthread_mutex_lock failed");
    job_scheduler->stop = true;
    error_handler(pthread_cond_broadcast(&job_scheduler->queue_not_empty) != 0,"pthread_cond_broadcast failed");
    error_handler(pthread_mutex_unlock(&job_scheduler->queue_mutex) != 0,"pthread_mutex_unlock failed");
    for( int i = 0 ; i < job_scheduler->number_of_threads ; i++ )
        error_handler(pthread_join(job_scheduler->thread_pool[i],0) != 0,"pthread_join failed");
}

// adds a job in the queue
void schedule_job_scheduler(struct job_scheduler *job_scheduler, void (*function)(void*), void *argument, int *barrier) {
    if( function == NULL )
        return;
    error_handler(job_scheduler == NULL,"job scheduler is NULL");
    error_handler(pthread_mutex_lock(&job_scheduler->queue_mutex) != 0,"pthread_mutex_lock failed");
    struct job *new_job = initialize_job(function,argument,barrier);
    push_tail_queue(&job_scheduler->queue, (void*)new_job);
    job_scheduler->jobs++;
    error_handler(pthread_cond_signal(&job_scheduler->queue_not_empty) != 0,"pthread_cond_signal failed");
    error_handler(pthread_cond_signal(&job_scheduler->barrier) != 0,"pthread_cond_signal failed");
    error_handler(pthread_mutex_unlock(&job_scheduler->queue_mutex) != 0,"pthread_mutex_unlock failed");
}

// pops a job from the queue and executes it
void execute_job(struct job_scheduler *job_scheduler) {
    struct job *job = NULL;
    void (*function)(void*), *argument;
    int *barrier;

    error_handler(job_scheduler == NULL,"job scheduler is NULL");

    job = (struct job *)pop_head_queue(&job_scheduler->queue);
    error_handler(pthread_mutex_unlock(&job_scheduler->queue_mutex) != 0,"pthread_mutex_unlock failed");

    function = job->function;
    argument = job->argument;
    // execute the function
    function(argument);

    error_handler(pthread_mutex_lock(&job_scheduler->queue_mutex) != 0,"pthread_mutex_lock failed");
    barrier = job->barrier;
    free_job(&job);
    job_scheduler->jobs--;
    if( job_scheduler->jobs == 0 )
        error_handler(pthread_cond_signal(&job_scheduler->queue_empty) != 0,"pthread_cond_signal failed");
    if( (*barrier) == 0 )
        error_handler(pthread_cond_signal(&job_scheduler->barrier) != 0,"pthread_cond_signal failed");
    error_handler(pthread_mutex_unlock(&job_scheduler->queue_mutex) != 0,"pthread_mutex_unlock failed");
}

// the new threads start execution by invoking this function
void *thread_function(void *job_scheduler_argument) {
    struct job_scheduler *job_scheduler = (struct job_scheduler *)job_scheduler_argument;

    error_handler(job_scheduler == NULL,"job scheduler is NULL");
    
    signal(ORIGINAL_SIGNAL, (void (*)(int))hold_threads);
    hold_threads(FAKE_SIGNAL, job_scheduler);

#if THREAD_POOL_DEBUG
    int thread_id = pthread_self();
    printnl(thread_id);
#endif

    while( true ) {
        error_handler(pthread_mutex_lock(&job_scheduler->queue_mutex) != 0,"pthread_mutex_lock failed");
        while( (job_scheduler->queue->length == 0) && (job_scheduler->stop == false) )
            pthread_cond_wait(&job_scheduler->queue_not_empty,&job_scheduler->queue_mutex);
        if( (job_scheduler->queue->length == 0) && (job_scheduler->stop == true) ) {
            error_handler(pthread_mutex_unlock(&job_scheduler->queue_mutex) != 0,"pthread_mutex_unlock failed");
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
        job_scheduler = (struct job_scheduler *)job_scheduler_argument;
    error_handler(job_scheduler == NULL,"job scheduler is NULL");
    if( signal == ORIGINAL_SIGNAL ) {
        error_handler(pthread_mutex_lock(&job_scheduler->pause_mutex) != 0,"pthread_mutex_lock failed");
        while( job_scheduler->pause == true )
            pthread_cond_wait(&job_scheduler->resume,&job_scheduler->pause_mutex);
        error_handler(pthread_mutex_unlock(&job_scheduler->pause_mutex) != 0,"pthread_mutex_unlock failed");
    }
}

// sends a signal to all threads in thread pool and they fell asleep util they get resumed
void pause_job_scheduler(struct job_scheduler *job_scheduler) {
    error_handler(job_scheduler == NULL,"job scheduler is NULL");
    error_handler(pthread_mutex_lock(&job_scheduler->pause_mutex) != 0,"pthread_mutex_lock failed");
    job_scheduler->pause = true;
    error_handler(pthread_mutex_unlock(&job_scheduler->pause_mutex) != 0,"pthread_mutex_unlock failed");
    for( int i = 0 ; i < job_scheduler->number_of_threads ; i++ )
        pthread_kill(job_scheduler->thread_pool[i], ORIGINAL_SIGNAL);
}

// resumes all threads in thread pool
void resume_job_scheduler(struct job_scheduler *job_scheduler) {
    error_handler(job_scheduler == NULL,"job scheduler is NULL");
    error_handler(pthread_mutex_lock(&job_scheduler->pause_mutex) != 0,"pthread_mutex_lock failed");
    job_scheduler->pause = false;
    error_handler(pthread_cond_broadcast(&job_scheduler->resume) != 0,"pthread_cond_broadcast failed");
    error_handler(pthread_mutex_unlock(&job_scheduler->pause_mutex) != 0,"pthread_mutex_unlock failed");
}