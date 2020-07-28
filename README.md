# Thread pool

A thread pool is a group of threads initially created that waits for jobs and executes them. The idea is to have the threads always existing, 
so that we won't have to pay overhead time for creating them every time. Also, the thread pool keeps the system from becoming overloaded. 
It allows a limit to the number of threads and tasks are queued and only run when a thread is available.

## Queue

Queue is a linear structure which follows a particular order in which the operations are performed. The order is First In First Out (FIFO). 
It holds the function to call and it's arguments. Basically, it is used for the temporary storage of jobs.

When I started the project I implemented a simple Queue, but I developed it to a **Generic Double Linked Queue** (Probably useful for other projects).

A **good approach when someone wants to use the Queue** is to create his own allocation, initialization and free functions for his struct. For example, 
my functions for job struct in job_scheduler.c file: a)initialize_job(allocates and initializes a job struct), b) free_job(frees a job struct).

```c
    // Queue functions
    struct queue *initialize_queue();
    void push_head_queue(struct queue **, void *);
    void push_tail_queue(struct queue **, void *);
    void *pop_head_queue(struct queue **);
    void *pop_tail_queue(struct queue **);
    void free_queue(struct queue **);
```

![queue](https://user-images.githubusercontent.com/48658768/87132434-862def00-c29e-11ea-9921-c21bd3173f19.png)

## Job Scheduler

Job scheduler is a structure that manages the thread pool and the queue. Job Scheduler's basic entity is Job. A Job is a routine, 
which we want to be executed by a thread, most likely parallel to some other. Any job can be defined as work and assigned to the Job Scheduler. 
The Scheduler basically accepts jobs and assigns them to threads.

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

![job_scheduler](https://user-images.githubusercontent.com/48658768/87144687-d4e48480-c2b0-11ea-9f2d-b7486d16820f.png)

**Each Job points to a Barrier** as you can see from the picture above, **which is just an integer that counts the sub-jobs of a Job**.
Imagine that the Main Thread added Job_1.1 and Job_1.2. **Barrier helps us to use the Main Thread like a Thread in Thread pool**
(dynamic_barrier_job_scheduler()). So, the Main Thread instead of waiting (barrier_job_scheduler()) executes jobs as well, until
it's jobs are finished. Of course, **the same thing happens to all the Threads in Thread pool**. They can add more Jobs as they
are executing a Job and instead of waiting them to finish they keep popping Jobs and executes them. Check multiple_jobs.c example/test.

## Testing
[**CUnit**](http://cunit.sourceforge.net/) is a lightweight system for writing, administering, and running unit tests in C. 
It provides C programmers a basic testing functionality with a flexible variety of user interfaces. That's what I used
to test my **Queue**. As for the **Job Scheduler**, I implemented some examples of it's use.

### Instalation instructions

```
    $ sudo apt-get install libcunit1-dev libcunit1-doc libcunit1
```

### Execution instructions

Run all the tests **together**

```
    $ cd tests/scripts
    $ ./run_tests.sh
```

Οr you can compile and run each executable **individually**

```
    $ cd tests/job_scheduler
    $ make
    $ valgrind ./'executable_name'
```

You can check out my [**Query Executor**](https://github.com/alexiszamanidis/query_executor) Project that uses this Thread Pool to parallelize it's tasks.