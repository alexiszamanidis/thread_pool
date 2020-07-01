#include "../../header/header.h"
#include "../../header/queue.h"
#include "../../header/job_scheduler.h"
#include "../header/test_functions.h"

int main(void) {
    int barrier = 0;
    struct job_scheduler *job_scheduler = initialize_job_scheduler(NUMBER_OF_THREADS);

    for( int i = 0 ; i < 100 ; i ++ ) {
        struct jobs_add_jobs *jobs_add_jobs_argument = my_malloc(struct jobs_add_jobs,1);
        error_handler(jobs_add_jobs_argument == NULL,"malloc failed");
        *jobs_add_jobs_argument = (struct jobs_add_jobs){ .job_scheduler = job_scheduler };
        schedule_job_scheduler(job_scheduler,jobs_add_jobs,jobs_add_jobs_argument,&barrier);
    }

    dynamic_barrier_job_scheduler(job_scheduler,&barrier);

    free_job_scheduler(job_scheduler);

    return SUCCESS;
}