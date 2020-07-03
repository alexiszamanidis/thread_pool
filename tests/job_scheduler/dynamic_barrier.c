#include "../../header/header.h"
#include "../../header/queue.h"
#include "../../header/job_scheduler.h"
#include "../header/job_scheduler_test_functions.h"

int main(void) {
    int barrier = 0;
    struct job_scheduler *job_scheduler = initialize_job_scheduler(NUMBER_OF_THREADS);

    pause_job_scheduler(job_scheduler);

    // make sure that thread pool is sleeping
    sleep(2);

    for( int i = 0 ; i < 100 ; i ++ ) {
        struct coordinates *coordinates = my_malloc(struct coordinates,1);
        error_handler(coordinates == NULL,"malloc failed");
        *coordinates = (struct coordinates){ .x = i, .y = i};
        schedule_job_scheduler(job_scheduler,argument_job,coordinates,&barrier);
        schedule_job_scheduler(job_scheduler,simple_job,NULL,&barrier);
    }

    // main thread executes all jobs
    dynamic_barrier_job_scheduler(job_scheduler,&barrier);

    resume_job_scheduler(job_scheduler);

    free_job_scheduler(job_scheduler);

    return SUCCESS;
}