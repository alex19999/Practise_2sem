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

typedef struct Full_Desriptor {
    int fd_1;
    int fd_2;
    int semid;
} descr;

void my_semop(int semid, int change_value) {
	struct sembuf mybuf;
	mybuf.sem_op = change_value;
	mybuf.sem_flg = 0;
	mybuf.sem_num = 0;
	if(semid < 0) {
		printf("Incorrect semid is given\n");
		exit(-1);
	}
	if(semop(semid, &mybuf, 1) < 0) {
		printf("Can't wait for condition\n");
		exit(-1);
	}
}

void* func_client(void* fd_1) {
    descr fd = *((descr*)fd_1);
    char *input;
    int num_of_bytes = 0;
    input = (char*)calloc(MAX_SYM, sizeof(char));
	printf("Please, wait for your queue, if it will be necessary\n");
	my_semop(fd.semid, -1);
	printf("Give me arguments to multiply\n");
    while (1) {
        fgets(input, MAX_SYM*sizeof(char), stdin);
        fd.fd_1 = open("fifo_to.fifo", O_WRONLY);
        if((num_of_bytes = write(fd.fd_1, input, MAX_SYM)) < 0) {
            printf("Can't write\n");
            exit(-1);
        }
        close(fd.fd_1);
        fd.fd_2 = open("fifo_in.fifo", O_RDONLY);
        if((num_of_bytes = read(fd.fd_2, input, MAX_SYM)) < 0) {
            printf("Can't read\n");
            exit(-1);
        }
        close(fd.fd_2);
        printf("result = %s\n", input);
        break;
    }
	my_semop(fd.semid, 1);
	free(input);
}


int calculate_serv(char* str_for_mult) {
    char* delimeter = "*";
    char** tokens;
    int* num_of_args;
    int iter = 0;
    int i = 0;
    int counter = 0;
    int result = 1;
    tokens = (char**)calloc(MAX_NUM_OF_ARGS, sizeof(char*));
    for(iter = 0; iter < MAX_NUM_OF_ARGS; iter++) {
        tokens[iter] = (char*)calloc(MAX_SYM, sizeof(char));
    }
    num_of_args = (int*)calloc(1, sizeof(int));
    Split(str_for_mult, delimeter, tokens, num_of_args);
    if(*num_of_args < 2) { 
   	    exit(0);
    } else {
        for(i = 0; i < *num_of_args; i++) {
            result = result * atoi(tokens[i]);
        }
    }
	free(num_of_args);
	for(counter = 0; counter < iter; counter++) {
		free(tokens[counter]);
	}
	free(tokens);
	return result;
}

void* func_server(void* fd_2){
    descr fd = *((descr*)fd_2);
    my_semop(fd.semid, MAX_CLIENTS);
    int num_of_bytes = 0; 
    int iter = 0;
    int result = 1;
    int counter = 0;
    char* res;
    char *output;
    output = (char*)calloc(MAX_SYM, sizeof(char));
    res = (char*)calloc(MAX_SYM, sizeof(char));
    do {
	    fd.fd_2 = open("fifo_to.fifo", O_RDONLY);
        if((num_of_bytes = read(fd.fd_2, output, MAX_SYM)) < 0) {
            printf("Can't read\n");
            exit(-1);
        }
        close(fd.fd_2);
        result = calculate_serv(output);
        sprintf(res, "%d", result);
        fd.fd_1 = open("fifo_in.fifo", O_WRONLY);
        if((num_of_bytes = write(fd.fd_1, res, MAX_SYM)) < 0) {
            printf("Can't write\n");
            exit(-1);
        }
        close(fd.fd_1);
	} while(1);
    free(res);
    free(output);
}

int main(int argc, char **argv) {
    if(argc == 2) {
        descr fd;
        unsigned int serv_or_client = atoi(argv[1]);
        int status_1;
        int status_2;
        pthread_t th_1;
        pthread_t th_2;
        char* filename_1 = "fifo_to.fifo";
        char* filename_2 = "fifo_in.fifo";
        int size = 1;
	    key_t key;
	    if((key = ftok("1.c", 0)) < 0) {
		printf("Can't get key\n");
		exit(-1);
	    }
	    if((fd.semid = semget(key, 1, 0666| IPC_CREAT)) < 0) {
		    printf("Can't get semid\n");
		    exit(-1);
	    }
        if(access(filename_1, F_OK) == -1)
        if (mknod(filename_1, S_IFIFO | 0666, 0) < 0) {
            exit(-1);
        }
        if(access(filename_2, F_OK) == -1)
        if (mknod(filename_2, S_IFIFO | 0666, 0) < 0) {
            exit(-1);
        }
        if(serv_or_client == CLIENT) {
                status_2 = pthread_create(&th_2, NULL, func_client, &fd);
                pthread_join(th_2, NULL);

        } else {
            printf("semid = %d\n", fd.semid);
            status_1 = pthread_create(&th_1, NULL, func_server, &fd);
            pthread_join(th_1, NULL);
        }
    } else {
        printf("Incorrect number of args\n");
    }
        return 0;
}


