CC = gcc
CFLAGS = -g -Wall
RMFLAGS = -rf
EXECS = simple_job multiple_jobs pause_resume global_variable jobs_add_jobs dynamic_barrier
OBJS = test_functions.o queue.o job_scheduler.o

all: $(EXECS) remove_objs usage

simple_job: $(OBJS) simple_job.c
	$(CC) $(CFLAGS) $(OBJS) simple_job.c -o simple_job -lpthread

multiple_jobs: $(OBJS) multiple_jobs.c
	$(CC) $(CFLAGS) $(OBJS) multiple_jobs.c -o multiple_jobs -lpthread

pause_resume: $(OBJS) pause_resume.c
	$(CC) $(CFLAGS) $(OBJS) pause_resume.c -o pause_resume -lpthread

global_variable: $(OBJS) global_variable.c
	$(CC) $(CFLAGS) $(OBJS) global_variable.c -o global_variable -lpthread

jobs_add_jobs: $(OBJS) jobs_add_jobs.c
	$(CC) $(CFLAGS) $(OBJS) jobs_add_jobs.c -o jobs_add_jobs -lpthread

dynamic_barrier: $(OBJS) dynamic_barrier.c
	$(CC) $(CFLAGS) $(OBJS) dynamic_barrier.c -o dynamic_barrier -lpthread

remove_objs:
	rm -rf $(OBJS)

test_functions.o: test_functions.c
	$(CC) $(CFLAGS) -c test_functions.c

queue.o: ../source/queue.c
	$(CC) $(CFLAGS) -c ../source/queue.c

job_scheduler.o: ../source/job_scheduler.c
	$(CC) $(CFLAGS) -c ../source/job_scheduler.c

usage:
	@echo 'Usage: valgrind ./simple_job || ./multiple_jobs || ./pause_resume || ./global_variable || ./jobs_add_jobs || ./dynamic_barrier'

clean:
	rm $(RMFLAGS) $(EXECS) $(OBJS)
