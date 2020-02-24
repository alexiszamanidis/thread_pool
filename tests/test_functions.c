#include "../header/test_functions.h"
#include "../header/job_scheduler.h"

void argument_job(void *argument) {
    struct test *test = (struct test *)argument;
    printf("x=%d, y=%d\n", test->x, test->y);
}

void simple_job(void *argument) {
    printf("no argument function\n");
}

void jobs_add_jobs(void *argument) {
    struct jobs_add_jobs *jobs_add_jobs_argument = (struct jobs_add_jobs *)argument;
    struct job_scheduler *job_scheduler = jobs_add_jobs_argument->job_scheduler;
    int barrier = 0;

    for( int i = 0 ; i < 100 ; i ++ ) {
        struct test *test = my_malloc(struct test,1);
        error_handler(test == NULL,"malloc failed");
        *test = (struct test){ .x = i, .y = i};
        schedule_job_scheduler(job_scheduler,(void*)argument_job,test,&barrier);
        schedule_job_scheduler(job_scheduler,(void*)simple_job,NULL,&barrier);
    }

    dynamic_barrier_job_scheduler(job_scheduler,&barrier);
}