#include "../header/job_scheduler.h"

// initializes the job scheduler with the number of open threads
struct job_scheduler *initialize_job_scheduler(int number_of_threads) {
    if( number_of_threads < 1 )
        error_handler("initialize_job_scheduler: number of threads should be greater than 0");

    struct job_scheduler *new_job_scheduler = (struct job_scheduler *)malloc(sizeof(struct job_scheduler));
    new_job_scheduler->thread_pool = malloc(sizeof(pthread_t)*number_of_threads);
    if( new_job_scheduler->thread_pool == NULL )
        error_handler("initialize_job_scheduler: malloc failed");

    new_job_scheduler->number_of_threads = number_of_threads;
    new_job_scheduler->jobs = 0;
    new_job_scheduler->stop = false;
    new_job_scheduler->pause = false;
    pthread_mutex_init(&new_job_scheduler->queue_mutex, NULL);
    pthread_cond_init(&new_job_scheduler->queue_empty, NULL);
    pthread_cond_init(&new_job_scheduler->queue_not_empty, NULL);
    pthread_mutex_init(&new_job_scheduler->pause_mutex, NULL);
    pthread_cond_init(&new_job_scheduler->resume, NULL);
    new_job_scheduler->queue = initialize_queue();
    create_threads_job_scheduler(new_job_scheduler);

    return new_job_scheduler;
}

// creates all the threads in thread pool
void create_threads_job_scheduler(struct job_scheduler *job_scheduler) {
    if( job_scheduler == NULL )
        error_handler("create_threads_job_scheduler");
    for( int i = 0 ; i < job_scheduler->number_of_threads ; i++ )
        if( pthread_create(&(job_scheduler->thread_pool[i]),0,thread_function,(void *) job_scheduler) != 0 )
            error_handler("create_threads_job_scheduler: pthread_create failed");
}

// waits until executed all jobs in the queue
void barrier_job_scheduler(struct job_scheduler *job_scheduler) {
    if( job_scheduler == NULL )
        error_handler("barrier_job_scheduler");
    pthread_mutex_lock(&job_scheduler->queue_mutex);
    while( job_scheduler->jobs > 0 )
        pthread_cond_wait(&job_scheduler->queue_empty,&job_scheduler->queue_mutex);
    pthread_mutex_unlock(&job_scheduler->queue_mutex);
}

// waits until executed all jobs from a specific barrier in the queue
void dynamic_barrier_job_scheduler(struct job_scheduler *job_scheduler, int *barrier) {
    if( job_scheduler == NULL )
        error_handler("dynamic_barrier_job_scheduler");
    
    while( (*barrier) != 0 ) {
        pthread_mutex_lock(&job_scheduler->queue_mutex);
        if( job_scheduler->queue->length !=0 )
            execute_job(job_scheduler);
        else
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
    pthread_mutex_destroy(&job_scheduler->pause_mutex);
    pthread_cond_destroy(&job_scheduler->resume);
    free_queue(&job_scheduler->queue);
    free(job_scheduler);
}

// waits until all threads finish their job and after that close all threads
void stop_job_scheduler(struct job_scheduler *job_scheduler) {
    if( job_scheduler == NULL )
        error_handler("stop_job_scheduler");
    job_scheduler->stop = true;
    pthread_cond_broadcast(&job_scheduler->queue_not_empty);
    for( int i = 0 ; i < job_scheduler->number_of_threads ; i++ )
        pthread_join(job_scheduler->thread_pool[i],0);
}

// adds a job in the queue
void schedule_job_scheduler(struct job_scheduler *job_scheduler, void (*function)(void*), void *argument, int *barrier) {
    if( job_scheduler == NULL )
        error_handler("schedule_job_scheduler");
    pthread_mutex_lock(&job_scheduler->queue_mutex);
    push_queue(&job_scheduler->queue, function, argument, barrier);
    job_scheduler->jobs++;
    pthread_cond_signal(&job_scheduler->queue_not_empty);
    pthread_mutex_unlock(&job_scheduler->queue_mutex);
}

// pops a job from the queue and executes it
void execute_job(struct job_scheduler *job_scheduler) {
    struct job *job = NULL;
    void (*function)(void*), *argument;

    if( job_scheduler == NULL )
        error_handler("execute_job");

    job = pop_queue(&job_scheduler->queue);
    pthread_mutex_unlock(&job_scheduler->queue_mutex);

    function = job->function;
    argument = job->argument;
    // execute the function
    function(argument);

    pthread_mutex_lock(&job_scheduler->queue_mutex);
    job_scheduler->jobs--;
    if( job_scheduler->jobs == 0 )
        pthread_cond_signal(&job_scheduler->queue_empty);
    free_job(&job);
    pthread_mutex_unlock(&job_scheduler->queue_mutex);
}

// the new threads start execution by invoking this function
void *thread_function(void *job_scheduler_argument) {
    struct job_scheduler *job_scheduler = job_scheduler_argument;
    struct sigaction signal_action = {0};

    if( job_scheduler == NULL )
        error_handler("thread_function");

    signal_action.sa_flags = 0;
    signal_action.sa_handler = hold_threads;
    if( sigaction(SIGUSR1, &signal_action, NULL) == -1 )
        error_handler("thread_function: sigaction failed");

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

// pause signal handler, holds threads until they resumed
void hold_threads() {
    extern struct job_scheduler *job_scheduler;
    if( job_scheduler == NULL )
        error_handler("hold_threads");
    pthread_mutex_lock(&job_scheduler->pause_mutex);
    while( job_scheduler->pause == true )
        pthread_cond_wait(&job_scheduler->resume,&job_scheduler->pause_mutex);
    pthread_mutex_unlock(&job_scheduler->pause_mutex);
}

// sends a signal to all threads in thread pool and they fell asleep util they get resumed
void pause_job_scheduler(struct job_scheduler *job_scheduler) {
    if( job_scheduler == NULL )
        error_handler("pause_job_scheduler");
    pthread_mutex_lock(&job_scheduler->pause_mutex);
    job_scheduler->pause = true;
    pthread_mutex_unlock(&job_scheduler->pause_mutex);
    for( int i = 0 ; i < job_scheduler->number_of_threads ; i++ )
        pthread_kill(job_scheduler->thread_pool[i], SIGUSR1);
}

// resumes all threads in thread pool
void resume_job_scheduler(struct job_scheduler *job_scheduler) {
    if( job_scheduler == NULL )
        error_handler("resume_job_scheduler");
    pthread_mutex_lock(&job_scheduler->pause_mutex);
    job_scheduler->pause = false;
    pthread_cond_broadcast(&job_scheduler->resume);
    pthread_mutex_unlock(&job_scheduler->pause_mutex);
}