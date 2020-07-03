#include "../../header/header.h"
#include "../../header/queue.h"
#include "../../header/job_scheduler.h"
#include "../header/job_scheduler_test_functions.h"

int main(void) {
    struct timespec begin, end;
    double time;
    int barrier = 0;
    struct job_scheduler *job_scheduler = initialize_job_scheduler(NUMBER_OF_THREADS);

    clock_gettime(CLOCK_MONOTONIC, &begin);

    pause_job_scheduler(job_scheduler);

    for( int i = 0 ; i < 100 ; i ++ ) {
        struct coordinates *coordinates = my_malloc(struct coordinates,1);
        error_handler(coordinates == NULL,"malloc failed");
        *coordinates = (struct coordinates){ .x = i, .y = i};
        schedule_job_scheduler(job_scheduler,argument_job,coordinates,&barrier);
        schedule_job_scheduler(job_scheduler,simple_job,NULL,&barrier);
    }

    sleep(5);
    resume_job_scheduler(job_scheduler);

    dynamic_barrier_job_scheduler(job_scheduler,&barrier);

    clock_gettime(CLOCK_MONOTONIC, &end);
    time_spent(time,begin,end);
    printf("Execution time = %f\n",time);

    free_job_scheduler(job_scheduler);

    return SUCCESS;
}