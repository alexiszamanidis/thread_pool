#ifndef header_H
#define header_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#define SUCCESS 0
#define FAILURE -1
#define NUMBER_OF_THREADS 2
#define ORIGINAL_SIGNAL SIGUSR1
#define FAKE_SIGNAL SIGUSR2

#define error_handler(expression, message)                                  \
    do {                                                                    \
        if( (expression) == true ) {                                        \
            fprintf(stderr, "%s:%u: %s: `" #expression "' %s.\n",           \
                    __FILE__, __LINE__, __func__,message);                  \
            exit(FAILURE);                                                  \
        }                                                                   \
    } while (0)

#define time_spent(time,begin,end)                                          \
    do {                                                                    \
        time = (end.tv_sec - begin.tv_sec);                                 \
        time = time + (end.tv_nsec-begin.tv_nsec)/1000000000.0;             \
    } while (0)

#endif