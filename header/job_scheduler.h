#ifndef job_scheduler_h_
#define job_scheduler_h_

#include "./header.h"
#include "./queue.h"

struct job_scheduler{
    int number_of_threads;
    int jobs;
    bool stop;
    bool pause;
    pthread_t *thread_pool;
    pthread_mutex_t queue_mutex;
    pthread_cond_t queue_empty;
    pthread_cond_t queue_not_empty;
    pthread_cond_t barrier;
    pthread_mutex_t pause_mutex;
    pthread_cond_t resume;
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
void hold_threads(const int , void *);
void pause_job_scheduler(struct job_scheduler *);
void resume_job_scheduler(struct job_scheduler *);

#endif