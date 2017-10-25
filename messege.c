#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define MAX_SYM 1000

void* func_write(void* fd_1) {
        int fd = *((int*)fd_1);
        char *input;
        input = (char*)calloc(MAX_SYM, sizeof(char));
                    while (1) {
                        fgets(input, MAX_SYM*sizeof(char), stdin);
                        if(strlen(input) < 1) continue;
                        else {
                            printf("Write\n");
                            write(fd, input, MAX_SYM*sizeof(char));
                        }
                    }
}

void* func_read(void* fd_2){
        int fd = *((int*)fd_2);
        int num_of_bytes = 0; 
        char *output;
        output = (char*)calloc(MAX_SYM, sizeof(char));
        do {
            printf("Read\n");
            num_of_bytes = read(fd, output, MAX_SYM);
            printf("messege:%s", output);
        }
        while (num_of_bytes > 0);
}
//Указываете два терминала, он их открывает и устраивает общение //

int main(int argc, char **argv) {
        int fd_1;
        int fd_2;
        unsigned int first_term = atoi(argv[1]);
        unsigned int second_term = atoi(argv[2]);
        int status_1;
        int status_2;
        pthread_t th_1;
        pthread_t th_2;
        char *input;
        char *output;
        char* filename_1 = "fifo_to.fifo";
        char* filename_2 = "fifo_in.fifo";
        int size =1;
        if(access(filename_1, F_OK) == -1)
        if (mknod(filename_1, S_IFIFO | 0666, 0) < 0) {
            exit(-1);
        }
        if(access(filename_2, F_OK)==-1)
        if (mknod(filename_2, S_IFIFO | 0666, 0) < 0) {
            exit(-1);
        }
        if (first_term != second_term) {
            if(first_term > second_term) {
                fd_1= open(filename_1, O_WRONLY);
                fd_2 = open(filename_2, O_RDONLY);
            } else {
                fd_2 = open(filename_1, O_RDONLY);
                fd_1= open(filename_2, O_WRONLY);
            }
        } else {
            printf("Use different tetminals to chat\n");
        }
        status_1 = pthread_create(&th_1, NULL, func_write, &fd_1);
        status_2 = pthread_create(&th_2, NULL, func_read, &fd_2);
        pthread_join(th_1, NULL);
        pthread_join(th_2, NULL);
        return 0;
}
