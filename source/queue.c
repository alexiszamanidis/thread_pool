#include "../header/queue.h"

// initialized the job with a function and argument
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

// initialized the queue
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

// push a job in the queue
void push_queue(struct queue **queue, void (*function)(void*), void *argument) {
    struct job *new_job = initialize_job(function,argument);

    // if the queue is empty, fix head and tail
    if( (*queue)->length == 0 ) {
        (*queue)->head = new_job;
        (*queue)->tail = new_job;
    }
    // otherwise fix only tail
    else {
        (*queue)->tail->next = new_job;
        (*queue)->tail = new_job;
    }
    (*queue)->length++;
}

// pop a job from the queue
struct job *pop_queue(struct queue **queue){
    struct job *temp_job = (*queue)->head;
    // if the queue is empty, then there is not data to be removed
    if( (*queue)->length == 0 ) {
        printf("There is no data to be removed.\n");
        return NULL;
    }
    // if the queue has only 1 data, fix head and tail
    else if( (*queue)->length == 1 ) {
        (*queue)->head = NULL;
        (*queue)->tail = NULL;
    }
    // otherwise move only head
    else
        (*queue)->head = (*queue)->head->next;
    (*queue)->length--;
    return temp_job;
}

// free all resources that are allocated by queue
void free_queue(struct queue **queue) {
    struct job *temp_job = NULL;

    if( queue == NULL )
        return;

    while((*queue)->head) {
        temp_job = (*queue)->head->next;
        free((*queue)->head);
        (*queue)->head = temp_job;
    }
    free(*queue);
}

void free_job(struct job **job) {
    if( job == NULL )
        return;
    if( (*job)->argument != NULL )
        free((*job)->argument);
    free(*job);
}