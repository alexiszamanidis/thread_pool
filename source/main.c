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
    free(test);
}

int main(void) {

    job_scheduler = initialize_job_scheduler(NUMBER_OF_THREADS);
    create_threads_job_scheduler();

    for( int i = 0 ; i < 10 ; i ++) {
        struct test *test = (struct test *)malloc(sizeof(struct test));
        test->x = i;
        test->y = i;
        schedule_job_scheduler((void*)test_function,test);
    }

    barrier_job_scheduler();
    stop_job_scheduler();
    free_job_scheduler();

    return SUCCESS;
}