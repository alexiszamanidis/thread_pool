#include "../header/queue.h"

struct job *initialize_job(void (*function)(void*), void *argument) {
    struct job *new_job = (struct job *)malloc(sizeof(struct job));
    if( new_job == NULL) {
        printf("initialize_job: %s\n",strerror(errno));
        exit(FAILURE);
    }

    new_job->function = function;
    new_job->argument = argument;
    new_job->next = NULL;
    
    return new_job;
}

struct queue *initialize_queue() {
    struct queue *new_queue = (struct queue *)malloc(sizeof(struct queue));
    if( new_queue == NULL) {
        printf("initialize_queue: %s\n",strerror(errno));
        exit(FAILURE);
    }

    new_queue->head = NULL;
    new_queue->tail = NULL;
    new_queue->length = 0;
    
    return new_queue;
}