#ifndef job_scheduler_H
#define job_scheduler_H

#include "./header.h"
#include "./queue.h"

struct job_scheduler{
    int number_of_threads;
    int jobs;
    bool stop;
    bool pause;
    pthread_t *thread_pool;
    pthread_mutex_t mutex;
    pthread_cond_t empty;
    pthread_cond_t not_empty;
    struct queue *queue;
};

struct job_scheduler *initialize_job_scheduler(int );
void create_threads_job_scheduler(struct job_scheduler *);
void barrier_job_scheduler(struct job_scheduler *);
void dynamic_barrier_job_scheduler(struct job_scheduler *, int *);
void free_job_scheduler(struct job_scheduler *);
void stop_job_scheduler(struct job_scheduler *);
void schedule_job_scheduler(struct job_scheduler *,void (*function)(void*), void *, int *);
void execute_job(struct job_scheduler *);
void *thread_function(void *);

#endif