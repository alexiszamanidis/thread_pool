#ifndef header_H
#define header_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>

#define error_handler(message)              \
    do {                                    \
        perror(message);                    \
        exit(FAILURE);                      \
    } while (0)

#define SUCCESS 0
#define FAILURE -1
#define NUMBER_OF_THREADS 2

#endif