#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include"split.h"
#define TABLE_LIMIT 2
#define MAX_SYM 15
#define MAX_NUM_OF_DISHES 100
#define MAX_TYPE_OF_DISHES 10

void* func_wash(void* fd_1){
    FILE* f_one = fopen("dirty.txt", "r");
    FILE* f_two = fopen("washing_time.txt", "r");
    FILE* mes_file = fopen("mes.txt", "wr+");
    int fd = *((int*)fd_1);
    int iter = 0;
    int k = 0;
    int* quantity;
    char* quantity_of_dish;
    char* time_for_washing;
    char* separator = ":";
    char** tokens_dirty_dish;
    char** tokens_dish_washtime;
    tokens_dirty_dish = (char**)calloc(MAX_NUM_OF_DISHES, sizeof(char*));
    tokens_dish_washtime = (char**)calloc(MAX_TYPE_OF_DISHES, sizeof(char*));
    quantity = (int*)calloc(1, sizeof(int));
    quantity_of_dish = (char*)calloc(MAX_SYM, sizeof(char));
    time_for_washing = (char*)calloc(MAX_SYM, sizeof(char));
    while(!feof(f_one)) {
        fgets(quantity_of_dish, MAX_SYM, f_one);
        quantity_of_dish[strlen(quantity_of_dish) - 1] = 0;
        printf("quantity_of_dish = %s\n", quantity_of_dish);
        Split(quantity_of_dish, separator, tokens_dirty_dish, quantity);
        printf("tokens_dirty_dish[0] =%s tokens_dirty_dish[1] = %s\n",tokens_dirty_dish[0], tokens_dirty_dish[1]);
        while(!feof(f_two)) {
            fgets(time_for_washing, MAX_SYM, f_two);
            time_for_washing[strlen(time_for_washing) - 1] = 0;
            printf("time_for_washing = %s\n", time_for_washing);
            Split(time_for_washing, separator, tokens_dish_washtime, quantity);
            printf("tokens_dish_washtime[0] = %s tokens_for_washing[1] = %s\n", tokens_dish_washtime[0], tokens_dish_washtime[1]);
            if(!strcmp(tokens_dish_washtime[0], tokens_dirty_dish[0])){
                for(iter = 0; iter < atoi(tokens_dirty_dish[1]); iter++){
                    sleep(atoi(tokens_dish_washtime[1]));
                    printf("%d\n", atoi(tokens_dish_washtime[1]));
                    printf("to fifo = %s\n", tokens_dirty_dish[0]);
                    printf("Washed\n");
                    //write(fd, tokens_dirty_dish[0], MAX_SYM*sizeof(char));
                    fputs(tokens_dirty_dish[0], mes_file);
                    fputc('\n', mes_file);
                }
                break;
               
            }
        }
        if(!feof(f_two)) rewind(f_two);
        rewind(mes_file);
    }
}

void* func_dry(void* fd_2){
    sleep(5);
    int fd = *((int*)fd_2);
    int counter = 0;
    int num_of_bytes = 0;
    int* quantity;
    FILE* f = fopen("dry_time.txt", "r");
    FILE* mes_file = fopen("mes.txt", "r");
    char* dish_for_drying;
    char* time_for_drying;
    char** tokens_dish_drytime;
    char* separator = ":";
    quantity = (int*)calloc(1, sizeof(int));
    dish_for_drying = (char*)calloc(MAX_SYM, sizeof(char));
    time_for_drying = (char*)calloc(MAX_SYM, sizeof(char));
    tokens_dish_drytime = (char**)calloc(2, sizeof(char*));
    do {
        fgets(dish_for_drying, MAX_SYM, mes_file);
        dish_for_drying[strlen(dish_for_drying) - 1] = 0;
        //num_of_bytes = read(fd, dish_for_drying, MAX_SYM*sizeof(char));
        //printf("num_of_bytes = %d\n", num_of_bytes);
        printf("dish_for_dry = %s\n", dish_for_drying);
        while(!feof(f)) {
            fgets(time_for_drying, MAX_SYM, f);
            time_for_drying[strlen(time_for_drying) - 1] = 0;
            //printf("time_for_drying = %s\n", time_for_drying);
            Split(time_for_drying, separator, tokens_dish_drytime, quantity);
            if(!strcmp(tokens_dish_drytime[0], dish_for_drying)) {
                    sleep(atoi(tokens_dish_drytime[1]));
                    printf("Dried\n");
                    break;
            } else {
                //printf("dont have any dish for drying\n");
            }
        }
        rewind(mes_file);
    }
    while (fgets(dish_for_drying, MAX_SYM, mes_file) != NULL);
    rewind(mes_file);
}

int main() {
    int status_1;
    int status_2;
    FILE* fd_1;
    FILE* fd_2;
    pthread_t th_1;
    pthread_t th_2;
    char* filename_1 = "fifo_to.fifo";
    char* filename_2 = "fifo_in.fifo";
    int size = 1; 
    if(access(filename_1, F_OK) == -1)
    if(mknod(filename_1, S_IFIFO | 0666, 0) < 0){
        exit(-1);
    }
    if(access(filename_2, F_OK)==-1)
    if (mknod(filename_2, S_IFIFO | 0666, 0) < 0) {
        exit(-1);
    }
    fd_1 = fopen(filename_1, "w+");
    printf("file = %p\n", fd_1);
    fd_2 = fopen(filename_1, "r");
    printf("file2 = %p\n", fd_2);
    printf("in main hello\n");
    status_1 = pthread_create(&th_1, NULL, func_wash, &fd_1);
    status_2 = pthread_create(&th_2, NULL, func_dry, &fd_2);
    pthread_join(th_1, NULL);
    pthread_join(th_2, NULL);
    return 0;
}











    



