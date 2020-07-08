#ifndef queue_functions_h_
#define queue_functions_h_

#include "../../header/header.h"
#include "../../header/queue.h"

#define SIZE 5

struct coordinates {
    int x;
    int y;
};

void initialize_queue_test();
void push_head_queue_test();
void push_tail_queue_test();
void pop_head_queue_test();
void pop_tail_queue_test();
void get_length_queue_test();

#endif