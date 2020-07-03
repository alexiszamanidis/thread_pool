#include "../header/queue.h"

// initializes the queue
struct queue *initialize_queue() {
    struct queue *new_queue = my_malloc(struct queue,1);
    error_handler(new_queue == NULL,"malloc failed");

    new_queue->head = NULL;
    new_queue->tail = NULL;
    new_queue->length = 0;

    return new_queue;
}

// pushes a node from the head in the queue
void push_head_queue(struct queue **queue, void *data) {
    error_handler(queue == NULL,"queue is NULL");

    // allocate queue node
    struct queue_node *new_queue_node = my_malloc(struct queue_node,1);
    error_handler(new_queue_node == NULL,"malloc failed");

    // initialize new node
    new_queue_node->data = data;
    new_queue_node->previous = NULL;

    // if the queue is empty, fix head and tail
    if( (*queue)->length == 0 ) {
        new_queue_node->next = NULL;
        (*queue)->head = (void *)new_queue_node;
        (*queue)->tail = (void *)new_queue_node;
    }
    // otherwise fix only tail
    else {
        new_queue_node->next = (*queue)->head;
        (*queue)->head->previous = (void *)new_queue_node;
        (*queue)->head = (void *)new_queue_node;
    }
    (*queue)->length++;
}

// pushes a node from the tail in the queue
void push_tail_queue(struct queue **queue, void *data) {
    error_handler(queue == NULL,"queue is NULL");

    // allocate queue node
    struct queue_node *new_queue_node = my_malloc(struct queue_node,1);
    error_handler(new_queue_node == NULL,"malloc failed");

    // initialize new node
    new_queue_node->data = data;
    new_queue_node->next = NULL;

    // if the queue is empty, fix head and tail
    if( (*queue)->length == 0 ) {
        new_queue_node->previous = NULL;
        (*queue)->head = (void *)new_queue_node;
        (*queue)->tail = (void *)new_queue_node;
    }
    // otherwise fix only tail
    else {
        new_queue_node->previous = (*queue)->tail;
        (*queue)->tail->next = (void *)new_queue_node;
        (*queue)->tail = (void *)new_queue_node;
    }
    (*queue)->length++;
}

// pops a queue_node from the head in the queue and returns the data(struct) from it
void *pop_head_queue(struct queue **queue) {
    error_handler(queue == NULL,"queue is NULL");

    // get queue node and hold struct node
    void *data = NULL;
    struct queue_node *temp_queue_node = (*queue)->head;
    data = temp_queue_node->data;

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
    else {
        (*queue)->head->next->previous = NULL;
        (*queue)->head = (*queue)->head->next;
    }

    (*queue)->length--;
    free_pointer(&temp_queue_node);

    return data;
}

// pops a queue_node from the tail in the queue and returns the data(struct) from it
void *pop_tail_queue(struct queue **queue) {
    error_handler(queue == NULL,"queue is NULL");

    // get queue node and hold struct node
    void *data = NULL;
    struct queue_node *temp_queue_node = (*queue)->tail;
    data = temp_queue_node->data;

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
    else {
        (*queue)->tail->previous->next = NULL;
        (*queue)->tail = (*queue)->tail->previous;
    }

    (*queue)->length--;
    free_pointer(&temp_queue_node);

    return data;
}

// frees all resources that are allocated by queue
void free_queue(struct queue **queue) {
    struct queue_node *temp_queue_node = NULL;

    if( *queue == NULL )
        return;

    while((*queue)->head) {
        temp_queue_node = (void *)(*queue)->head->next;
        free_pointer(&(*queue)->head->data);
        free_pointer(&(*queue)->head);
        (*queue)->head = (void *)temp_queue_node;
    }
    free_pointer(queue);
}