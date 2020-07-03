CC = gcc
CFLAGS = -g -Wall
RMFLAGS = -rf
EXEC = main
OBJS = queue.o cunit.o queue_test_functions.o
CUNIT = -lcunit

all: $(EXEC) usage

main: $(OBJS) main.c
	$(CC) $(CFLAGS) $(OBJS) main.c $(CUNIT) -o main
	rm $(RMFLAGS) $(OBJS)

queue.o: ../../source/queue.c
	$(CC) $(CFLAGS) -c ../../source/queue.c

cunit.o: ../source/cunit.c
	$(CC) $(CFLAGS) -c ../source/cunit.c

queue_test_functions.o: queue_test_functions.c
	$(CC) $(CFLAGS) -c queue_test_functions.c

usage:
	@echo "Usage: valgrind ./main"

clean:
	rm $(RMFLAGS) $(EXEC) $(OBJS)
