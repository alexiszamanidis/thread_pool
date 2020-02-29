#ifndef queue_h_
#define queue_h_

#include "./header.h"

struct job{
    void   (*function)(void* argument);
    void*  argument;
    int *barrier;
    struct job *next;
};

struct queue{	
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

#endif