#include "../header/queue.h"

struct job *initialize_job(void (*function)(void*), void *argument) {
    struct job *new_job = (struct job *)malloc(sizeof(struct job));
    if( new_job == NULL) {
        perror("initialize_job failed");
        return NULL;
    }

    new_job->function = function;
    new_job->argument = argument;
    new_job->next = NULL;
    
    return new_job;
}