# Thread pool

## Queue

```c
    // Queue functions
    struct job *initialize_job(void (*function)(void*), void *);
    struct queue *initialize_queue();
    void push_queue(struct queue **, void (*function)(void*), void *);
    struct job *pop_queue(struct queue **);
    void free_queue(struct queue **);
```

## Job scheduler

```c
    // Job scheduler functions
    struct job_scheduler *initialize_job_scheduler(int );
    void create_threads_job_scheduler();
    void barrier_job_scheduler();
    void free_job_scheduler();
    void stop_job_scheduler();
    void schedule_job_scheduler(void (*function)(void*), void *);
    void *thread_function(void *);
```