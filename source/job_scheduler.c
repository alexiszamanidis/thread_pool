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