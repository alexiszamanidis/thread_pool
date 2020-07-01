#ifndef queue_h_
#define queue_h_

#include "./header.h"

struct queue_node {
    void *data;
    struct queue_node *next;
};

struct queue {
    struct queue_node *head;
    struct queue_node *tail;
    int length;
};

struct queue *initialize_queue();
void push_queue(struct queue **, void *);
void *pop_queue(struct queue **);
void free_queue(struct queue **);
void free_queue_node(struct queue_node **);

#endif