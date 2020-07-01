#ifndef queue_h_
#define queue_h_

#include "./header.h"

struct job {
    void (*function)(void *argument);
    void *argument;
    int *barrier;
    struct job *next;
};

struct generic_queue_node {
    void *data;
    struct generic_queue_node *next;
};

struct generic_queue {
    struct generic_queue_node *head;
    struct generic_queue_node *tail;
    int length;
    size_t size;
};

struct queue {
    struct job *head;
    struct job *tail;
    int length;
};

struct job *initialize_job(void (*function)(void*), void *, int *);
struct queue *initialize_queue();
bool push_queue(struct queue **, void (*function)(void*), void *, int *);
struct job *pop_queue(struct queue **);
void free_queue(struct queue **);
void free_job(struct job **);

struct generic_queue *initialize_generic_queue(size_t );
void push_generic_queue(struct generic_queue **, void *);
void *pop_generic_queue(struct generic_queue **);
void free_generic_queue(struct generic_queue **);
void free_generic_queue_node(struct generic_queue_node **);

#endif