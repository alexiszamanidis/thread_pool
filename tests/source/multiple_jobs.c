#include "../../header/header.h"
#include "../../header/queue.h"
#include "../../header/job_scheduler.h"
#include "../header/job_scheduler_test_functions.h"

int main(void) {
    int barrier = 0;
    struct job_scheduler *job_scheduler = initialize_job_scheduler(NUMBER_OF_THREADS);

    for( int i = 0 ; i < 100 ; i ++ ) {
        struct test *test = my_malloc(struct test,1);
        error_handler(test == NULL,"malloc failed");
        *test = (struct test){ .x = i, .y = i};
        schedule_job_scheduler(job_scheduler,argument_job,test,&barrier);
        schedule_job_scheduler(job_scheduler,simple_job,NULL,&barrier);
    }

    dynamic_barrier_job_scheduler(job_scheduler,&barrier);

    free_job_scheduler(job_scheduler);

    return SUCCESS;
}