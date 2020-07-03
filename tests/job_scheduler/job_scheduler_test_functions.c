#include "../header/job_scheduler_test_functions.h"
#include "../../header/job_scheduler.h"

void argument_job(void *argument) {
    struct coordinates *coordinates = (struct coordinates *)argument;
    printf("x=%d, y=%d\n", coordinates->x, coordinates->y);
}

void simple_job(void *argument) {
    printf("no argument function\n");
}

void jobs_add_jobs(void *argument) {
    struct jobs_add_jobs *jobs_add_jobs_argument = (struct jobs_add_jobs *)argument;
    struct job_scheduler *job_scheduler = jobs_add_jobs_argument->job_scheduler;
    int barrier = 0;

    for( int i = 0 ; i < 100 ; i ++ ) {
        struct coordinates *coordinates = my_malloc(struct coordinates,1);
        error_handler(coordinates == NULL,"malloc failed");
        *coordinates = (struct coordinates){ .x = i, .y = i};
        schedule_job_scheduler(job_scheduler,argument_job,coordinates,&barrier);
        schedule_job_scheduler(job_scheduler,simple_job,NULL,&barrier);
    }

    dynamic_barrier_job_scheduler(job_scheduler,&barrier);
}