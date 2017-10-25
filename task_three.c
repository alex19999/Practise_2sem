#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

// fixit: удаляйте лишние пустые строки

int main() {
        int N = 0;
        int iter = 0;
        int *status;
        pid_t pid;
        printf("give the amount of processes\n");
        scanf("%d", &N);
        for(iter = 0; iter <N; iter++) {
                pid = fork();
                if(pid == 0) {
                        printf("id of current process = %d\n", getpid());
                        printf("id of parent process = %d\n", getppid());
                } else {
                        wait(status);
                        printf("status = %d\n", *status);
                        exit(0);
                }
        }
}

