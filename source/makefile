CC = gcc
CFLAGS = -g -Wall
RMFLAGS = -rf
EXEC = thread_pool
OBJS = queue.o job_scheduler.o

all: main usage

main: $(OBJS) main.c
	$(CC) $(CFLAGS) $(OBJS) main.c -o $(EXEC) -lpthread
	rm -rf $(OBJS)

queue.o: queue.c
	$(CC) $(CFLAGS) -c queue.c

job_scheduler.o: job_scheduler.c
	$(CC) $(CFLAGS) -c job_scheduler.c

usage:
	@echo 'Usage of thread_pool: valgrind ./thread_pool'

clean:
	rm $(RMFLAGS) $(EXEC)
