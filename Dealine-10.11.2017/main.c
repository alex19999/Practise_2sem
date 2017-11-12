#include "wash_wipe.h"


int main() {
    int status_1;
    int status_2;
    int fd_1;
    int fd_2;
    pthread_t th_1;
    pthread_t th_2;
    char* filename_1 = "fifo_to.fifo";
    int size = 1; 
    if(access(filename_1, F_OK) == -1)
    if(mknod(filename_1, S_IFIFO | 0666, 0) < 0){
        exit(-1);
    }
    status_1 = pthread_create(&th_1, NULL, func_wash, &fd_1);
    status_2 = pthread_create(&th_2, NULL, func_wipe, &fd_2);
    pthread_join(th_1, NULL);
    pthread_join(th_2, NULL);
    return 0;
}











    



