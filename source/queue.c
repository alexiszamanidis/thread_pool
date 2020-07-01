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

// pushes a node in the queue
void push_queue(struct queue **queue, void *data) {
    error_handler(queue == NULL,"queue is NULL");

    // allocate queue node
    struct queue_node *new_queue_node = my_malloc(struct queue_node,1);
    error_handler(new_queue_node == NULL,"malloc failed");

    // initialize new node
    new_queue_node->data = data;
    new_queue_node->next = NULL;

    // if the queue is empty, fix head and tail
    if( (*queue)->length == 0 ) {
        (*queue)->head = (void *)new_queue_node;
        (*queue)->tail = (void *)new_queue_node;
    }
    // otherwise fix only tail
    else {
        (*queue)->tail->next = (void *)new_queue_node;
        (*queue)->tail = (void *)new_queue_node;
    }
    (*queue)->length++;
}

// pops a queue_node from the queue and returns the data(struct) from it
void *pop_queue(struct queue **queue) {
    error_handler(queue == NULL,"queue is NULL");

    struct queue_node *temp_queue_node = (*queue)->head;
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

    return temp_queue_node;
}

// frees all resources that are allocated by queue
void free_queue(struct queue **queue) {
    struct queue_node *temp_queue_node = NULL;

    if( *queue == NULL )
        return;

    while((*queue)->head) {
        temp_queue_node = (void *)(*queue)->head->next;
        free((*queue)->head->data);
        free((*queue)->head);
        (*queue)->head = (void *)temp_queue_node;
    }
    free_pointer(queue);
}

// frees a queue node and the allocated struct it has
void free_queue_node(struct queue_node **queue_node) {
    if( *queue_node == NULL )
        return;

    free((*queue_node)->data);
    free((*queue_node));
}