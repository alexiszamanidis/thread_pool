#ifndef queue_H
#define queue_H

#include "./header.h"

struct job{
    void   (*function)(void* argument);
    void*  argument;
    struct job *next;
};

struct queue{	
    struct job *head;
    struct job *tail;
    int length;
};

struct job *initialize_job(void (*function)(void*), void *);
struct queue *initialize_queue();

#endif