#include "../../header/header.h"
#include "../../header/queue.h"
#include "../../header/job_scheduler.h"
#include "../../header/test_functions.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int global_variable = 0;

void increase() {
	pthread_mutex_lock(&mutex);
	global_variable++;
	pthread_mutex_unlock(&mutex);
}

int main(void) {
    int barrier = 0;
    struct job_scheduler *job_scheduler = initialize_job_scheduler(NUMBER_OF_THREADS);

    for( int i = 0 ; i < 100 ; i ++ )
        schedule_job_scheduler(job_scheduler,increase,NULL,&barrier);

    dynamic_barrier_job_scheduler(job_scheduler,&barrier);

    printnl(global_variable);

    free_job_scheduler(job_scheduler);

    return SUCCESS;
}