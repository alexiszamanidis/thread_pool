CC = gcc
CFLAGS = -g -Wall
RMFLAGS = -rf
EXEC = thread_pool

all: main usage

main: main.c
	$(CC) $(CFLAGS) main.c -o $(EXEC) -lpthread

usage:
	@echo 'Usage of thread_pool: valgrind ./thread_pool'

clean:
	rm $(RMFLAGS) $(EXEC)
