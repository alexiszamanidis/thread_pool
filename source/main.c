#include "../header/header.h"
#include "../header/queue.h"
#include "../header/job_scheduler.h"

struct test {
    int x;
    int y;
};

void test_function(void *argument) {
    struct test *test = argument;
    printf("x=%d, y=%d\n", test->x, test->y);
}

void test_function_2(void *argument) {
    printf("no argument function\n");
}

int main(void) {
    struct timespec begin, end;
    double time;
    int barrier = 0;
    struct job_scheduler *job_scheduler = initialize_job_scheduler(NUMBER_OF_THREADS);

    clock_gettime(CLOCK_MONOTONIC, &begin);

    for( int i = 0 ; i < 10 ; i ++) {
        struct test *test = (struct test *)malloc(sizeof(struct test));
        error_handler(test == NULL,"malloc failed");
        test->x = i;
        test->y = i;
        schedule_job_scheduler(job_scheduler,(void*)test_function,test,&barrier);
        schedule_job_scheduler(job_scheduler,(void*)test_function_2,NULL,&barrier);
    }

    pause_job_scheduler(job_scheduler);
    sleep(5);
    resume_job_scheduler(job_scheduler);

    barrier_job_scheduler(job_scheduler);

    clock_gettime(CLOCK_MONOTONIC, &end);
    time_spent(time,begin,end);
    printf("Execution time = %f\n",time);

    stop_job_scheduler(job_scheduler);
    free_job_scheduler(job_scheduler);

    return SUCCESS;
}