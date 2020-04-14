CC = gcc
CFLAGS = -g -Wall #-DTHREAD_POOL_DEBUG
TFLAG = -lpthread
RMFLAGS = -rf
EXECS = simple_job multiple_jobs pause_resume global_variable jobs_add_jobs dynamic_barrier null_function_pointer
OBJS = test_functions.o queue.o job_scheduler.o

all: $(EXECS) remove_objs usage

simple_job: $(OBJS) simple_job.c
	$(CC) $(CFLAGS) $(OBJS) simple_job.c -o simple_job $(TFLAG)

multiple_jobs: $(OBJS) multiple_jobs.c
	$(CC) $(CFLAGS) $(OBJS) multiple_jobs.c -o multiple_jobs $(TFLAG)

pause_resume: $(OBJS) pause_resume.c
	$(CC) $(CFLAGS) $(OBJS) pause_resume.c -o pause_resume $(TFLAG)

global_variable: $(OBJS) global_variable.c
	$(CC) $(CFLAGS) $(OBJS) global_variable.c -o global_variable $(TFLAG)

jobs_add_jobs: $(OBJS) jobs_add_jobs.c
	$(CC) $(CFLAGS) $(OBJS) jobs_add_jobs.c -o jobs_add_jobs $(TFLAG)

dynamic_barrier: $(OBJS) dynamic_barrier.c
	$(CC) $(CFLAGS) $(OBJS) dynamic_barrier.c -o dynamic_barrier $(TFLAG)

null_function_pointer: $(OBJS) null_function_pointer.c
	$(CC) $(CFLAGS) $(OBJS) null_function_pointer.c -o null_function_pointer $(TFLAG)

remove_objs:
	rm -rf $(OBJS)

test_functions.o: test_functions.c
	$(CC) $(CFLAGS) -c test_functions.c

queue.o: ../../source/queue.c
	$(CC) $(CFLAGS) -c ../../source/queue.c

job_scheduler.o: ../../source/job_scheduler.c
	$(CC) $(CFLAGS) -c ../../source/job_scheduler.c

usage:
	@echo 'Usage: valgrind ./simple_job || ./multiple_jobs || ./pause_resume || ./global_variable || ./jobs_add_jobs || ./dynamic_barrier || ./null_function_pointer'

clean:
	rm $(RMFLAGS) $(EXECS) $(OBJS)
