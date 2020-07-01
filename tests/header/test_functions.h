#ifndef test_functions_h_
#define test_functions_h_

#include "../../header/header.h"

struct test {
    int x;
    int y;
};

struct jobs_add_jobs {
    struct job_scheduler *job_scheduler;
};

void argument_job(void *);
void simple_job(void *);
void jobs_add_jobs(void *);

#endif