#include "calc_server.h"
#include <sys/msg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void my_semop(int semid, int change_value) {
    if(semid < 0) {
        printf("Incorrect semid\n");
        exit(-1);
    }
    struct sembuf mybuf;
    mybuf.sem_op = change_value;
    mybuf.sem_flg = 0;
    mybuf.sem_num = 0;
    semop(semid, &mybuf, 1);
}


void my_send_mes(int msqid, char* message) {
    int result = 0;
    int len = 0;
    if(msqid < 0 || message == NULL) {
        printf("Problems with message\n");
        exit(-1);
    }
    struct msgbuf {
        long mtype;
        char mtext[1024];
    } mybuf;
    mybuf.mtype = 1;
    strcpy(mybuf.mtext, message);
    len = strlen(mybuf.mtext) + 1;
    if((result = msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0)) < 0) {
        printf("Can't send message\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }
}

void my_receive_mes(int msqid, char* message) {
    int result = 0;
    int max_len = 20;
    if(msqid < 0) {
        printf("Problems with msqid\n");
        exit(-1);
    }
    struct msgbuf {
        long mtype;
        char mtext[1024];
    } mybuf;
    mybuf.mtype = 1;
    strcpy(mybuf.mtext, message);
    if((result = msgrcv(msqid, (struct msgbuf *) &mybuf, max_len, 0, 0)) < 0) {
        printf("Can't receive message\n");
        exit(-1);
    }
    strcpy(message, mybuf.mtext);
}


void func_client(descr fd) {
    char *input;
    int num_of_bytes = 0;
    input = (char*)calloc(MAX_SYM, sizeof(char));
    while (1) {
        fgets(input, MAX_SYM*sizeof(char), stdin);
	    my_send_mes(fd.msqid, input);
	    my_receive_mes(fd.msqid, input);
        printf("result = %s\n", input);
		break;
    }
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
    int num_of_bytes = 0; 
    int iter = 0;
    int result = 1;
    int counter = 0;
    char* res;
    char* output;
    output = (char*)calloc(100, sizeof(char));
    res = (char*)calloc(MAX_SYM, sizeof(char));
    do {
        my_receive_mes(fd.msqid, output);
	    result = calculate_serv(output);
        sprintf(res, "%d", result);
		my_send_mes(fd.msqid, res);
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
        int size = 1;
	    key_t key;
	    if((key = ftok("1.c", 0)) < 0) {
		    printf("Can't get key\n");
		    exit(-1);
	    }
        if((fd.semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
            printf("Can't get semid\n");
            exit(-1);
        }
	    if((fd.msqid = msgget(key, 0666| IPC_CREAT)) < 0) {
		    printf("Can't get msqid\n");
		    exit(-1);
	    }
        if(serv_or_client == CLIENT) {
            printf("Please, wait, if it will be needed\n");
            my_semop(fd.semid, -1);
            printf("Give me arguments to multiply\n");
            func_client(fd);
            my_semop(fd.semid, 1);
        } else {
            my_semop(fd.semid, 2);
            printf("semid = %d\n", fd.semid);
            while(1) {
                status_1 = pthread_create(&th_1, NULL, func_server, &fd);
                pthread_join(th_1, NULL);
            }
        }
    } else {
        printf("Incorrect number of args\n");
    }
        return 0;
}


