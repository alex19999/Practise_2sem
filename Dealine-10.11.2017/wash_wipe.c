#include "wash_wipe.h"

int num_of_washed_dishes = 0;

void* func_wash(void* fd_1){
    FILE* f_one = fopen("dirty.txt", "r");
    FILE* f_two = fopen("washing_time_2.txt", "r");
    int fd = *((int*)fd_1);
    int iter = 0;
    int k = 0;
    int* quantity;
    char* pointer;
    char* dirty_dish;
    char* time_for_washing;
    char* separator = ":";
    char** tokens_dirty_dish;
    char** tokens_dish_washtime;
    fd = open("fifo_to.fifo", O_WRONLY);
    tokens_dirty_dish = (char**)calloc(NUM_OF_ARG, sizeof(char*));
    tokens_dish_washtime = (char**)calloc(NUM_OF_ARG, sizeof(char*));
    quantity = (int*)calloc(1, sizeof(int));
    dirty_dish = (char*)calloc(MAX_SYM, sizeof(char));
    time_for_washing = (char*)calloc(MAX_SYM, sizeof(char));
    do{
        pointer = fgets(dirty_dish, MAX_SYM, f_one);
        if(pointer != NULL) {
            dirty_dish[strlen(dirty_dish) - 1] = 0;
            Split(dirty_dish, separator, tokens_dirty_dish, quantity);
            while(!feof(f_two)) {
                fgets(time_for_washing, MAX_SYM, f_two);
                time_for_washing[strlen(time_for_washing) - 1] = 0;
                Split(time_for_washing, separator, tokens_dish_washtime, quantity);
                if(!strcmp(tokens_dish_washtime[0], tokens_dirty_dish[0])){
                    for(iter = 0; iter < atoi(tokens_dirty_dish[1]); iter++){
                        while(num_of_washed_dishes >= 2) sleep(0.1);
                        sleep(atoi(tokens_dish_washtime[1]));
                        printf("%s is washed\n", tokens_dirty_dish[0]);
                        write(fd, tokens_dirty_dish[0], MAX_SYM*sizeof(char));
                        num_of_washed_dishes++;
                    }
                    break;
                }  
            }
            rewind(f_two);
        }
    }
    while(!feof(f_one));
    num_of_washed_dishes = -1;
    free(tokens_dirty_dish);
    free(tokens_dish_washtime);
    free(time_for_washing);
    free(dirty_dish);
    free(quantity);
    fclose(f_one);
    fclose(f_two);
}

void* func_wipe(void* fd_2){
    int fd = *((int*)fd_2);
    int num_of_bytes = 0;
    int* quantity;
    FILE* f = fopen("dry2.txt", "r");
    char* dish_for_wiping;
    char* time_for_wiping;
    char* separator = ":";
    char** tokens_dish_wipetime;
    fd = open("fifo_to.fifo", O_RDONLY);
    quantity = (int*)calloc(1, sizeof(int));
    dish_for_wiping = (char*)calloc(MAX_SYM, sizeof(char));
    time_for_wiping = (char*)calloc(MAX_SYM, sizeof(char));
    tokens_dish_wipetime = (char**)calloc(2, sizeof(char*));
    do {
        if(num_of_washed_dishes < 0) break;
        num_of_bytes = read(fd, dish_for_wiping, MAX_SYM);
        while(!feof(f)) {
            fgets(time_for_wiping, MAX_SYM, f);
            time_for_wiping[strlen(time_for_wiping) - 1] = 0;
            Split(time_for_wiping, separator, tokens_dish_wipetime, quantity);
            if(!strcmp(tokens_dish_wipetime[0], dish_for_wiping)) {
                    sleep(atoi(tokens_dish_wipetime[1]));
                    num_of_washed_dishes--;
                    printf("%s is wiped\n", dish_for_wiping);
                    break;
            }
        }
        rewind(f);
        }
    while(num_of_bytes > 0);
    free(dish_for_wiping);
    free(time_for_wiping);
    free(tokens_dish_wipetime);
    free(quantity);
    fclose(f);
}
