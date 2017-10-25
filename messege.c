#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

/*
fixit:
Т.к. это упражнение перешло в домашнее, его надо сделать нормально:
1) название переменных, из которых ясно, что в них хранится
2) убрать дублирование кода (запуск клиента 1 очень схож с запуском 2го).
*/

#define MAX_SYM 1000

void* func_write(void* fd_1){
        int fd = *((int*)fd_1);
        char *input;
        input = (char*)calloc(MAX_SYM, sizeof(char));
                    while (1) {
                        fgets(input, MAX_SYM*sizeof(char), stdin);
                        /*
                        Можете пояснить зачем нужна строчка
                        if(strlen(input)<1) sleep(0.1);
                        ?
                        */
                        if(strlen(input)<1) sleep(0.1);
                        else {
                            printf("Write\n");
                            write(fd, input, MAX_SYM*sizeof(char));
                        }
                    }
}

void* func_read(void* fd_2){
        int fd = *((int*)fd_2);
        int status = 0; // fixit: у вас переменная status фактически означает число прочитанных байт ... может так и назвать?
        char *output;
        output = (char*)calloc(MAX_SYM, sizeof(char));
        do {
            printf("Read\n");
            status = read(fd, output, MAX_SYM);
            printf("messege:%s", output);
        }
        while (status > 0);
}

int main(int argc, char **argv){
        /*
        fixit: дублирующийся код с точностью до замены суффикса переменных _1 на _2
        */
        int fd_1;
        int fd_2;
        int status_1;
        int status_2;
        pthread_t th_1;
        pthread_t th_2;
        char *input;
        char *output;
        char *filename_1 = "fifo_to.fifo";
        char *filename_2 = "fifo_in.fifo";
        int size =1;
        if(access(filename_1, F_OK) == -1)
        if (mknod(filename_1, S_IFIFO | 0666, 0) < 0) {
            exit(-1);
        }
        if(access(filename_2, F_OK)==-1)
        if (mknod(filename_2, S_IFIFO | 0666, 0) < 0) {
            exit(-1);
        }
        if (atoi(argv[1]) == 0) {
            fd_1= open(filename_1, O_WRONLY);
            status_1 = pthread_create(&th_1, NULL, func_write, &fd_1);
            fd_2 = open(filename_2, O_RDONLY);
            status_2 = pthread_create(&th_2, NULL, func_read, &fd_2);
            pthread_join(th_1, NULL);
            pthread_join(th_2, NULL);
        }
        if (atoi(argv[1]) == 1) {
            fd_2 = open(filename_1, O_RDONLY);
            fd_1= open(filename_2, O_WRONLY);
            status_1 = pthread_create(&th_1, NULL, func_write, &fd_1);
            status_2 = pthread_create(&th_2, NULL, func_read, &fd_2);
            pthread_join(th_1, NULL);
            pthread_join(th_2, NULL);
        }
        return 0;
}
