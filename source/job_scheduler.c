#include "../header/job_scheduler.h"

struct job_scheduler *initialize_job_scheduler(int number_of_threads) {
    if( number_of_threads < 1) {
        printf("initialize_job_scheduler: %s\n",strerror(errno));
        exit(FAILURE);
    }

    struct job_scheduler *new_job_scheduler = (struct job_scheduler *)malloc(sizeof(struct job_scheduler));
    new_job_scheduler->thread_pool = malloc(sizeof(pthread_t)*number_of_threads);
    if( new_job_scheduler->thread_pool == NULL) {
        printf("initialize_job_scheduler: %s\n",strerror(errno));
        exit(FAILURE);
    }

    new_job_scheduler->number_of_threads = number_of_threads;
    new_job_scheduler->jobs = 0;
    new_job_scheduler->stop = false;
    pthread_mutex_init(&new_job_scheduler->mutex, NULL);
    pthread_cond_init(&new_job_scheduler->empty, NULL);
    pthread_cond_init(&new_job_scheduler->not_empty, NULL);
    new_job_scheduler->queue = initialize_queue();

    return new_job_scheduler;
}

void create_threads_job_scheduler() {
    extern struct job_scheduler *job_scheduler;

    for( int i = 0 ; i < job_scheduler->number_of_threads ; i++ )
        pthread_create(&(job_scheduler->thread_pool[i]),0,thread_function,0);
}

void barrier_job_scheduler() {
    extern struct job_scheduler *job_scheduler;

    pthread_mutex_lock(&job_scheduler->mutex);
    while( job_scheduler->jobs > 0 )
        pthread_cond_wait(&job_scheduler->empty,&job_scheduler->mutex);
    pthread_mutex_unlock(&job_scheduler->mutex);
}

void free_job_scheduler() {
    extern struct job_scheduler *job_scheduler;

    free((job_scheduler)->thread_pool);
    pthread_mutex_destroy(&(job_scheduler)->mutex);
    pthread_cond_destroy(&(job_scheduler)->empty);
    pthread_cond_destroy(&(job_scheduler)->not_empty);
    free_queue(&(job_scheduler)->queue);
    free(job_scheduler);
}

void stop_job_scheduler() {
    extern struct job_scheduler *job_scheduler;

    job_scheduler->stop = true;
    pthread_cond_broadcast(&job_scheduler->not_empty);
    for( int i = 0 ; i < (job_scheduler)->number_of_threads ; i++ )
        pthread_join((job_scheduler)->thread_pool[i],0);
}

void schedule_job_scheduler(void (*function)(void*), void *argument) {
    extern struct job_scheduler *job_scheduler;

    pthread_mutex_lock(&job_scheduler->mutex);
    push_queue(&job_scheduler->queue, function, argument);
    job_scheduler->jobs++;
    pthread_cond_signal(&job_scheduler->not_empty);
    pthread_mutex_unlock(&job_scheduler->mutex);
}

void *thread_function(void *arguments) {
    extern struct job_scheduler *job_scheduler;
    struct job *job = NULL;
    void (*function)(void*);
    void*  argument;

    while( true ) {
        pthread_mutex_lock(&job_scheduler->mutex);
        while( (job_scheduler->queue->length == 0) && (job_scheduler->stop == false) )
            pthread_cond_wait(&job_scheduler->not_empty,&job_scheduler->mutex);
        if( job_scheduler->stop == true ) {
            pthread_mutex_unlock(&job_scheduler->mutex);
            pthread_exit(0);
        }
        else {
            job = pop_queue(&job_scheduler->queue);
            pthread_mutex_unlock(&job_scheduler->mutex);

            function = job->function;
            argument = job->argument;

            function(argument);

            free(job);

            pthread_mutex_lock(&job_scheduler->mutex);
            job_scheduler->jobs--;
            if( job_scheduler->jobs == 0 )
                pthread_cond_signal(&job_scheduler->empty);
            pthread_mutex_unlock(&job_scheduler->mutex);
        }
    }
}