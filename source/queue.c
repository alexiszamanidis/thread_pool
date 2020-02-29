#include "../header/queue.h"

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

// initializes the queue
struct queue *initialize_queue() {
    struct queue *new_queue = my_malloc(struct queue,1);
    error_handler(new_queue == NULL,"malloc failed");

    new_queue->head = NULL;
    new_queue->tail = NULL;
    new_queue->length = 0;
    
    return new_queue;
}

// pushes a job in the queue
bool push_queue(struct queue **queue, void (*function)(void*), void *argument, int *barrier) {
    error_handler(queue == NULL,"queue is NULL");

    struct job *new_job = initialize_job(function,argument,barrier);
    if( new_job == NULL )
        return false;

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

    return true;
}

// pops a job from the queue
struct job *pop_queue(struct queue **queue){
    error_handler(queue == NULL,"queue is NULL");

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

// frees all resources that are allocated by queue
void free_queue(struct queue **queue) {
    struct job *temp_job = NULL;

    if( *queue == NULL )
        return;

    while((*queue)->head) {
        temp_job = (*queue)->head->next;
        free((*queue)->head);
        (*queue)->head = temp_job;
    }
    free_pointer(queue);
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