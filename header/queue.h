#ifndef queue_H
#define queue_H

#include "./header.h"

typedef struct job{
    void   (*function)(void* argument);
    void*  argument;
    struct job *next;
};

struct job *initialize_job(void (*function)(void*), void *);

#endif