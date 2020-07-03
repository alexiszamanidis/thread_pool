#include "../../header/header.h"
#include "../../header/queue.h"
#include "../header/cunit.h"
#include "../header/queue_test_functions.h"

int main(void) {
    CU_pSuite pSuite = NULL;

    // initialize the CUnit test registry
    if( CU_initialize_registry() != CUE_SUCCESS )
      return CU_get_error();

    // add a suite to the registry
    pSuite = CU_add_suite("vector_test_suite", 0, 0);
    if( pSuite == NULL ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // add the tests to the suite
    add_test(pSuite, "initialize_queue_test", initialize_queue_test);
    add_test(pSuite, "push_head_queue_test", push_head_queue_test);
    add_test(pSuite, "push_tail_queue_test", push_tail_queue_test);
    add_test(pSuite, "pop_head_queue_test", pop_head_queue_test);
    add_test(pSuite, "pop_tail_queue_test", pop_tail_queue_test);

    // run all tests using the CUnit Basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}