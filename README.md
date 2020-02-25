# Thread pool

A thread pool is a group of threads initially created that waits for jobs and executes them. The idea is to have the threads always existing, so that we won't have to pay overhead time for creating them every time. Also, the thread pool keeps the system from becoming overloaded. It allows a limit to the number of threads and tasks are queued and only run when a thread is available.

![thread_pool](https://user-images.githubusercontent.com/48658768/71448039-5d4d6e80-273f-11ea-9017-2e57e1b5971d.png)

## Queue

Queue is a linear structure which follows a particular order in which the operations are performed. The order is First In First Out (FIFO). It holds the function to call and it's arguments. Βasically, it is used for the temporary storage of jobs.

```c
    // Queue functions
    struct job *initialize_job(void (*function)(void*), void *, int *);
    struct queue *initialize_queue();
    void push_queue(struct queue **, void (*function)(void*), void *, int *);
    struct job *pop_queue(struct queue **);
    void free_queue(struct queue **);
    void free_job(struct job **);
```

## Job scheduler

Job scheduler is a structure that manages the thread pool and the queue. Job scheduler's basic entity is Job. A Job is a routine, which we want to be executed by a thread, most likely parallel to some other. Any job can be defined as work and assigned to the Job scheduler. The scheduler basically accepts jobs and assigns them to threads.

```c
    // Job scheduler functions
    struct job_scheduler *initialize_job_scheduler(int );
    void barrier_job_scheduler(struct job_scheduler *);
    void dynamic_barrier_job_scheduler(struct job_scheduler *, int *);
    void free_job_scheduler(struct job_scheduler *);
    void stop_job_scheduler(struct job_scheduler *);
    void schedule_job_scheduler(struct job_scheduler *,void (*function)(void*), void *, int *);
    void execute_job(struct job_scheduler *);
    void *thread_function(void *);
    void hold_threads(const int , void *);
    void pause_job_scheduler(struct job_scheduler *);
    void resume_job_scheduler(struct job_scheduler *);
```

### Installing && Updating

```
    $ git clone https://github.com/alexiszamanidis/thread_pool.git
    $ git pull
```

### Execution instructions

```
    $ cd tests
    $./memory_leaks.sh
           or
    $ make
    $ valgrind ./simple_job || ./multiple_jobs || ./pause_resume || ./global_variable
                            || ./jobs_add_jobs || ./dynamic_barrier
```

