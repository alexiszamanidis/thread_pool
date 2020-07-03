#ifndef queue_cunit_functions_h_
#define queue_cunit_functions_h_

#include "../../header/header.h"
#include "../../header/queue.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#define SIZE 5

struct coordinates {
    int x;
    int y;
};

void add_test(CU_pSuite , const char* , CU_TestFunc );
void initialize_queue_test();
void push_head_queue_test();
void push_tail_queue_test();
void pop_head_queue_test();
void pop_tail_queue_test();

#endif