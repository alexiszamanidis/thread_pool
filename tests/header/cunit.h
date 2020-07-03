#ifndef cunit_h_
#define cunit_h_

#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#define FAILURE -1

void add_test(CU_pSuite , const char* , CU_TestFunc );

#endif