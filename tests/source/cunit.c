#include "../header/cunit.h"

void add_test(CU_pSuite pSuite, const char* strName, CU_TestFunc pTestFunc) {
    if ( NULL == CU_add_test(pSuite, strName, pTestFunc) ) {
        CU_cleanup_registry();
        exit(FAILURE);
    }
}