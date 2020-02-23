#include "../header/header.h"
#include "../header/queue.h"
#include "../header/job_scheduler.h"
#include "../header/test.h"

int main(void) {
    int barrier = 0;
    struct job_scheduler *job_scheduler = initialize_job_scheduler(NUMBER_OF_THREADS);

    for( int i = 0 ; i < 100 ; i ++) {
        struct jobs_add_jobs *jobs_add_jobs_argument = (struct jobs_add_jobs *)malloc(sizeof(struct jobs_add_jobs));
        error_handler(jobs_add_jobs_argument == NULL,"malloc failed");
        *jobs_add_jobs_argument = (struct jobs_add_jobs){ .job_scheduler = job_scheduler };
        schedule_job_scheduler(job_scheduler,(void*)jobs_add_jobs,jobs_add_jobs_argument,&barrier);
    }

    dynamic_barrier_job_scheduler(job_scheduler,&barrier);

    stop_job_scheduler(job_scheduler);
    free_job_scheduler(job_scheduler);

    return SUCCESS;
}