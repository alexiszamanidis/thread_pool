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
    pthread_mutex_init(&new_job_scheduler->mutex, NULL);
    pthread_cond_init(&new_job_scheduler->empty, NULL);
    pthread_cond_init(&new_job_scheduler->not_empty, NULL);
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
    pthread_mutex_lock(&job_scheduler->mutex);
    while( job_scheduler->jobs > 0 )
        pthread_cond_wait(&job_scheduler->empty,&job_scheduler->mutex);
    pthread_mutex_unlock(&job_scheduler->mutex);
}

// waits until executed all jobs from a specific barrier in the queue
void dynamic_barrier_job_scheduler(struct job_scheduler *job_scheduler, int *barrier) {
    if( job_scheduler == NULL )
        error_handler("dynamic_barrier_job_scheduler");
    
    while( (*barrier) != 0 ) {
        pthread_mutex_lock(&job_scheduler->mutex);
        if( job_scheduler->queue->length !=0 )
            execute_job(job_scheduler);
        else
            pthread_mutex_unlock(&job_scheduler->mutex);
    }
}

// free all resources that are allocated by job scheduler
void free_job_scheduler(struct job_scheduler *job_scheduler) {
    if( job_scheduler == NULL )
        return;
    if( (job_scheduler)->thread_pool != NULL )
        free((job_scheduler)->thread_pool);

    pthread_mutex_destroy(&(job_scheduler)->mutex);
    pthread_cond_destroy(&(job_scheduler)->empty);
    pthread_cond_destroy(&(job_scheduler)->not_empty);
    free_queue(&(job_scheduler)->queue);
    free(job_scheduler);
}

// waits until all threads finish their job and after that close all threads
void stop_job_scheduler(struct job_scheduler *job_scheduler) {
    if( job_scheduler == NULL )
        error_handler("stop_job_scheduler");
    job_scheduler->stop = true;
    pthread_cond_broadcast(&job_scheduler->not_empty);
    for( int i = 0 ; i < (job_scheduler)->number_of_threads ; i++ )
        pthread_join((job_scheduler)->thread_pool[i],0);
}

// adds a job in the queue
void schedule_job_scheduler(struct job_scheduler *job_scheduler, void (*function)(void*), void *argument, int *barrier) {
    if( job_scheduler == NULL )
        error_handler("schedule_job_scheduler");
    pthread_mutex_lock(&job_scheduler->mutex);
    push_queue(&job_scheduler->queue, function, argument, barrier);
    job_scheduler->jobs++;
    pthread_cond_signal(&job_scheduler->not_empty);
    pthread_mutex_unlock(&job_scheduler->mutex);
}

// pops a job from the queue and executes it
void execute_job(struct job_scheduler *job_scheduler) {
    struct job *job = NULL;
    void (*function)(void*), *argument;

    if( job_scheduler == NULL )
        error_handler("execute_job");

    job = pop_queue(&job_scheduler->queue);
    pthread_mutex_unlock(&job_scheduler->mutex);

    function = job->function;
    argument = job->argument;
    // execute the function
    function(argument);

    pthread_mutex_lock(&job_scheduler->mutex);
    job_scheduler->jobs--;
    if( job_scheduler->jobs == 0 )
        pthread_cond_signal(&job_scheduler->empty);
    free_job(&job);
    pthread_mutex_unlock(&job_scheduler->mutex);
}

// the new threads start execution by invoking this function
void *thread_function(void *job_scheduler_argument) {
    struct job_scheduler *job_scheduler = job_scheduler_argument;

    if( job_scheduler == NULL )
        error_handler("thread_function");

    while( true ) {
        pthread_mutex_lock(&job_scheduler->mutex);
        while( (job_scheduler->queue->length == 0) && (job_scheduler->stop == false) )
            pthread_cond_wait(&job_scheduler->not_empty,&job_scheduler->mutex);
        if( job_scheduler->stop == true ) {
            pthread_mutex_unlock(&job_scheduler->mutex);
            pthread_exit(0);
        }
        else
            execute_job(job_scheduler);
    }
}