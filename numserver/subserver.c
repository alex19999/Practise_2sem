#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "split.h"

#define MAX_SYM 1000
#define MAX_CLIENTS 5
#define MAX_NUM_OF_ARGS 10
#define CLIENT 1
#define SERVER 0

typedef struct Full_Desriptor {
    int who;
    int fd_1;
    int fd_2;
    char* result;
} descr;

void* func_write(void* fd_1) {
        descr fd = *((descr*)fd_1);
        char *input;
        input = (char*)calloc(MAX_SYM, sizeof(char));
        //if(fd.who == CLIENT) write(fd.fd_1, "connect", MAX_SYM*sizeof(char));
        while (1) {
            fgets(input, MAX_SYM*sizeof(char), stdin);
            while(strlen(input) < 1) sleep(0.1);
            if(fd.who == CLIENT) {
                fd.fd_1 = open("fifo_to.fifo", O_WRONLY);
                write(fd.fd_1, input, MAX_SYM*sizeof(char));
            }
            close(fd.fd_1);
        }
}

int calculate_serv(char* operation) {
	char* delimeter = "*";
    char** tokens;
    int* num_of_args;
    int iter = 0;
    int result = 1;
    tokens = (char**)calloc(MAX_NUM_OF_ARGS, sizeof(char*));
    for(iter = 0; iter < MAX_NUM_OF_ARGS; iter++) {
        tokens[iter] = (char*)calloc(MAX_SYM, sizeof(char));
    }
	num_of_args = (int*)calloc(1, sizeof(int));
	Split(operation, delimeter, tokens, num_of_args);
    if(*num_of_args < 2) { 
   		exit(0);
    } else {
        for(iter = 0; iter < *num_of_args; iter++) {
            result = result * atoi(tokens[iter]);
        }
    }
	return result;
}

void* func_read(void* fd_2){
    descr fd = *((descr*)fd_2);
    int num_of_bytes = 0; 
    int iter = 0;
    int result = 0;
    int counter = 0;
    char* res;
    char *output;
    output = (char*)calloc(MAX_SYM, sizeof(char));
    res = (char*)calloc(MAX_SYM, sizeof(char));
    fd.result = (char*)calloc(MAX_SYM, sizeof(char));
    do {
        if(fd.who == SERVER) fd.fd_2 = open("fifo_to.fifo", O_RDONLY);
        else fd.fd_2 = open("fifo_in.fifo", O_RDONLY);
        num_of_bytes = read(fd.fd_2, output, MAX_SYM);
        printf("result:%s\n", output);
        if(fd.who == SERVER) {
            //if(!strcmp(output, "connect")) counter++;
                result = calculate_serv(output);
                sprintf(res, "%d", result);
                fd.fd_1 = open("fifo_in.fifo", O_WRONLY);
                write(fd.fd_1, res, MAX_SYM*sizeof(char));
                close(fd.fd_1);
        }
        printf("counter = %d\n", counter);
        close(fd.fd_2);
    }
    while (num_of_bytes > 0);
}

int count_of_clients(FILE* f) {
    if (f == NULL) printf("Ошибка открытия файла\n");
  	else {
    	fseek(f, 0, SEEK_END);                                     
    	long size = ftell(f); 
	}                                                                                       
 
}




int main(int argc, char **argv) {
    if(argc == 2) {
        descr fd;
        unsigned int serv_or_client = atoi(argv[1]);
        int status_1;
        int status_2;
        pthread_t th_1;
        pthread_t th_2;
        char *input;
        char *output;
        char* result;
        char* filename_1 = "fifo_to.fifo";
        char* filename_2 = "fifo_in.fifo";
        int size = 1;
        result = (char*)calloc(MAX_SYM, sizeof(char));
        if(access(filename_1, F_OK) == -1)
        if (mknod(filename_1, S_IFIFO | 0666, 0) < 0) {
            exit(-1);
        }
        if(access(filename_2, F_OK) == -1)
        if (mknod(filename_2, S_IFIFO | 0666, 0) < 0) {
            exit(-1);
        }
        printf("here\n");
        if(serv_or_client == CLIENT) {
                //fd.fd_1 = open(filename_1, O_WRONLY);
                //fd.fd_2 = open(filename_2, O_RDONLY);
                fd.who = CLIENT;
                printf("Give me numbers to calculate, please\n");

        } else {
            //fd.fd_2 = open(filename_1, O_RDONLY);
            //fd.fd_1 = open(filename_2, O_WRONLY);
            fd.who = SERVER;
        }
        status_1 = pthread_create(&th_1, NULL, func_write, &fd);
        status_2 = pthread_create(&th_2, NULL, func_read, &fd);
        pthread_join(th_1, NULL);
        pthread_join(th_2, NULL);
    } else {
        printf("Incorrect number of args\n");
    }
        return 0;
}
