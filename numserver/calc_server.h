#ifndef _CALC_SERVER_H_
#define _CALC_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "split.h"

#define MAX_SYM 1000
#define MAX_CLIENTS 5
#define MAX_NUM_OF_ARGS 10
#define CLIENT 1
#define SERVER 0

typedef struct Full_Desriptor { // Structure, that will be given in pthread_create;
    int fd_1;
    int fd_2;
    int semid;
} descr;

void my_semop(int semid, int change_value); // Just my semop to simplify my programm;

void* func_client(void* fd_1); // Client function: give arguments in FIFO and receive them from FIFO as result;

int calculate_serv(char* str_for_mult);// Multiply given arguments;

void* func_server(void* fd_2); // Server function: takes arguments from FIFO, multiply and return them;

#endif /* _CALC_SERVER_H_ */

