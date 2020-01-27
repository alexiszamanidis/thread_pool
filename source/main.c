#include "../header/header.h"
#include "../header/queue.h"
#include "../header/job_scheduler.h"

struct job_scheduler *job_scheduler = NULL;

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
    double time_spent;

    job_scheduler = initialize_job_scheduler(NUMBER_OF_THREADS);
    create_threads_job_scheduler();

    clock_gettime(CLOCK_MONOTONIC, &begin);

    for( int i = 0 ; i < 10 ; i ++) {
        struct test *test = (struct test *)malloc(sizeof(struct test));
        test->x = i;
        test->y = i;
        schedule_job_scheduler((void*)test_function,test);
        schedule_job_scheduler((void*)test_function_2,NULL);
    }

    barrier_job_scheduler();

    clock_gettime(CLOCK_MONOTONIC, &end);
    time_spent = (end.tv_sec - begin.tv_sec);
    time_spent = time_spent + (end.tv_nsec-begin.tv_nsec)/1000000000.0;
    printf("Execution time = %f\n",time_spent);

    stop_job_scheduler();
    free_job_scheduler();

    return SUCCESS;
}