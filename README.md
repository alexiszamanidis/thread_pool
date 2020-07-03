# Thread pool

A thread pool is a group of threads initially created that waits for jobs and executes them. The idea is to have the threads always existing, 
so that we won't have to pay overhead time for creating them every time. Also, the thread pool keeps the system from becoming overloaded. 
It allows a limit to the number of threads and tasks are queued and only run when a thread is available.

## Queue

Queue is a linear structure which follows a particular order in which the operations are performed. The order is First In First Out (FIFO). 
It holds the function to call and it's arguments. Basically, it is used for the temporary storage of jobs.

```c
    // Queue functions
    struct queue *initialize_queue();
    void push_head_queue(struct queue **, void *);
    void push_tail_queue(struct queue **, void *);
    void *pop_head_queue(struct queue **);
    void *pop_tail_queue(struct queue **);
    void free_queue(struct queue **);
```

![queue](https://user-images.githubusercontent.com/48658768/86474114-d65efb80-bd4a-11ea-8d47-d27b61c8f9b8.png)

## Job scheduler

Job scheduler is a structure that manages the thread pool and the queue. Job scheduler's basic entity is Job. A Job is a routine, 
which we want to be executed by a thread, most likely parallel to some other. Any job can be defined as work and assigned to the Job scheduler. 
The scheduler basically accepts jobs and assigns them to threads.

```c
    // Job scheduler functions
    struct job *initialize_job(void (*function)(void*), void *, int *);
    void free_job(struct job **);
    struct job_scheduler *initialize_job_scheduler(int );
    void create_threads_job_scheduler(struct job_scheduler *);
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

![thread_pool](https://user-images.githubusercontent.com/48658768/71448039-5d4d6e80-273f-11ea-9017-2e57e1b5971d.png)

## Unit Testing
[CUnit](http://cunit.sourceforge.net/) is a lightweight system for writing, administering, and running unit tests in C.  
It provides C programmers a basic testing functionality with a flexible variety of user interfaces.

### Instalation instructions

```
    $ sudo apt-get install libcunit1-dev libcunit1-doc libcunit1
```

### Execution instructions

Run all the tests together

```
    $ cd tests
    $ ./run_tests.sh
```

Οr you can compile and run each executable individually

```
    $ cd tests/job_scheduler
    $ make
    $ valgrind ./'executable_name'
```