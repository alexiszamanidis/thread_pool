#ifndef job_scheduler_H
#define job_scheduler_H

#include "./header.h"
#include "./queue.h"

struct job_scheduler{
    int number_of_threads;
    int jobs;
    bool stop;
    pthread_t *thread_pool;
    pthread_mutex_t mutex;
    pthread_cond_t empty;
    pthread_cond_t not_empty;
    struct queue *queue;
};

#endif