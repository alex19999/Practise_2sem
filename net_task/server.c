#include"server.h"

int my_write(int descriptor, char* message, int newsockfd, int sockfd) {
    int num_of_bytes;
    if((num_of_bytes = write(descriptor, message, strlen(message))) < 0) {
        perror("Can't write");
        close(sockfd);
        close(newsockfd);
    }
    return num_of_bytes;
}

int my_read(int descriptor, char* destiny, int max_length, int newsockfd, int sockfd) {
    int num_of_bytes;
    if((num_of_bytes = read(descriptor, destiny, max_length)) < 0) {
        perror("Can't read");
        close(sockfd);
        close(newsockfd);
    }
    return num_of_bytes;
}

void map_constructing(int newsockfd, int sockfd, char* line, int who) {
    int fd = 0;
    if(who == CLIENT_1) fd = open("f1.txt", O_WRONLY); 
    if(who == CLIENT_2) fd = open("f2.txt", O_WRONLY); 
    int num_of_bytes = 0;
    int counter = 0;
    num_of_bytes = my_write(fd, line, newsockfd, sockfd);
    while((num_of_bytes = read(newsockfd, line, MAX_SYM)) > 0){
	printf("line = %s\n", line);
        num_of_bytes = my_write(fd, line, newsockfd, sockfd);
        counter++;
        printf("counter = %d\n", counter);
        if(counter >= 9) break;
    }
    close(fd);
}


int search_map(FILE* f) {
    char suspect;
    int alive = 0;
    while(!feof(f)) {
        suspect = fgetc(f);
        if(suspect == 'o') {
            alive++;
        }
    }
    rewind(f);
    return alive;
}

int check(int* coord, int* arr_1, int* arr_2, int length) {
    int iter = 0;
    if(coord == NULL) return 0;
    for(iter = 0; iter < length; iter++) {
        if(arr_1[iter] == coord[0]) {
            if(arr_2[iter] == coord[1]) {
                return 0;
            }
        }
    }
    return 1;
}

int hit_or_miss(int* coord, FILE* f) {
    if(coord == NULL) return 0;
    char alive = 'o';
    char suspect;
    int offset = 0;
    if(coord[0] > 0 && coord[0] <= 10){
        if(coord[1] > 0 && coord[1] <= 10){
            offset = (coord[0] - 1)*10 + coord[1] + coord[0] - 2;
            fseek(f, offset, SEEK_SET);
            suspect = fgetc(f);
            if(suspect == alive) {
                return 1;
            } else {
                return -1;
            }
            rewind(f);
        } else {
            perror(NULL);
            exit(1);
        }
    } else {
        perror(NULL);
        exit(1);
    }
}

int check_map(FILE* f, int newsockfd, int sockfd) {
    char brick = '*';
    char alive = 'o';
    int num_of_bricks = 0;
    char suspect;
    while(!feof(f)) {
        suspect = fgetc(f);
        if(!isspace(suspect)) {
            if(brick != suspect && alive != suspect) {
                perror("Checking map is failed");
                return -1;
                close(sockfd);
                close(newsockfd);
                exit(1);
            }
            num_of_bricks++;
        }
        if(num_of_bricks == 100) break;
    }
    rewind(f);
    if(num_of_bricks != CORRECT_NUM_OF_BRICKS) {
        perror("Checking the number of bricks is failed");
        return -1;
        close(sockfd);
        close(newsockfd);
        exit(1);
    }
    return 1;
}

void show_file(FILE* f) {
    int iter = 0;
    while(!feof(f)) {
        for(iter = 0; iter < 10; iter++){
            printf("%c", fgetc(f));
        }
        fputc('\n',stdin);
    }
}

int battle_preparing(int newsockfd, int sockfd, int* user_1, int* user_2) {
    FILE* f1 = fopen("f1.txt", "r");
    FILE* f2 = fopen("f2.txt", "r");
    char* message;
    int len_of_mess = 0;
    int num_of_bytes = 0;
    int enemy = 0;
    int units = 0;
    message = (char*)calloc(MAX_SYM, sizeof(char));
    close(user_1[0]);
    close(user_2[1]);
    len_of_mess = my_write(user_1[1], "ready", newsockfd, sockfd);
    while(len_of_mess = read(user_2[0], message, MAX_SYM) < 0) sleep(0.1);
    num_of_bytes = my_write(newsockfd, "ready", newsockfd, sockfd);
    check_map(f1, newsockfd, sockfd);
    enemy = search_map(f2);
    units = search_map(f1);
    if(units != enemy) {
        perror("Cheating, game will stoped");
        my_write(newsockfd, "end", newsockfd, sockfd);
        close(sockfd);
        close(newsockfd);
    }
    return units;
    fclose(f1);
    fclose(f2);
    free(message);
}

int* get_coordinates(int newsockfd, int* arr_str, int* arr_col, int* length) {
    int num_of_bytes = 0;
    int check_ = 0;
    int counter = 0;
    char* line;
    char* delimeter = " ";
    char** tokens;
    int* quantity;
    int* coordinates;
    tokens = (char**)calloc(2, sizeof(char));
    for(counter = 0; counter < 2; counter++) {
        tokens[counter] = (char*)calloc(1, sizeof(char));
    }	
    line = (char*)calloc(MAX_SYM, sizeof(char));
    quantity = (int*)calloc(1, sizeof(int));
    coordinates = (int*)calloc(2, sizeof(int));
    while((num_of_bytes = read(newsockfd, line, MAX_SYM)) < 0) sleep(0.1);
    Split(line, delimeter, tokens, quantity);
    coordinates[0] = atoi(tokens[0]);
    coordinates[1] = atoi(tokens[1]);
    if((check_ = check(coordinates, arr_str, arr_col, *length)) == 0) return NULL;
    arr_str[*length] = coordinates[0];
    arr_col[*length] = coordinates[1];
    *length++;
    if(line != NULL) free(line);
    for(counter = 0; counter < 2; counter++) {
        if(tokens[counter] != NULL) free(tokens[counter]);
    }
    free(tokens);
    return coordinates;
}

void send_result(int result,int newsockfd, int sockfd, int* user_fd) {
    int len_of_mess = 0;
    int num_of_bytes = 0;
    if(result == 1) {
        num_of_bytes = my_write(newsockfd, "Hitting", newsockfd, sockfd);
        } else {
            num_of_bytes = my_write(newsockfd, "Missing", newsockfd, sockfd);
        }
    len_of_mess = my_write(user_fd[1], "shoot", newsockfd, sockfd);
}

void end_of_the_game(int newsockfd, int sockfd, int* user_fd) {
    int len_of_mess = 0;
    int num_of_bytes = 0;
    len_of_mess = my_write(user_fd[1], "GameOver", newsockfd, sockfd);
    num_of_bytes = my_write(newsockfd, "Victory", newsockfd, sockfd);
}
    
void main() {
    int sockfd;
    int newsockfd;
    int clilen;
    int length = 0;
    int len_of_mess = 0;
    int result = 0;
    int units = 0;
    int user_fd_1[2];
    int user_fd_2[2];
    int num_of_bytes = 0;
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    char* message;
    char* line;
    int* arr_str;
    int* arr_col;
    int the_num_of_dead = 0;
    int is_continue = 0;
    int* coord;
    pid_t pid;
    line = (char*)calloc(MAX_SYM, sizeof(char));
    arr_col = (int*)calloc(MAX_SYM, sizeof(int));
    arr_str = (int*)calloc(MAX_SYM, sizeof(int));
    coord = (int*)calloc(2, sizeof(int));
    message = (char*)calloc(MAX_SYM, sizeof(char));
    if(pipe(user_fd_1) < 0 || pipe(user_fd_2)) {
        printf("Can't create pipe");
        exit(1);
    }
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket stage has a problem");
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51367);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
        printf("Bind stage has a problem");
        close(sockfd);
        exit(1);
    }
    if(listen(sockfd, 5) < 0) {
        printf("Listen stage hs a problem");
        close(sockfd);
        exit(1);
    }
    pid = fork();
    while(1) {
        clilen = sizeof(cliaddr);
        if(pid == 0) {
            if((newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen)) < 0) {
                printf("Accept stage has a problem");
                close(sockfd);
                exit(1);
            }
            while((num_of_bytes = read(newsockfd, line, MAX_SYM+2)) > 0) {
                map_constructing(newsockfd, sockfd, line, 1);
                units = battle_preparing(newsockfd, sockfd, user_fd_1, user_fd_2);
                FILE* f1 = fopen("f1.txt", "r");
                FILE* f2 = fopen("f2.txt", "r");
                while(1) {
                    coord = get_coordinates(newsockfd, arr_str, arr_col, &length);
                    result = hit_or_miss(coord, f2);
                    if(result == 1) {
                        the_num_of_dead++;
                        if(the_num_of_dead == units) { 
                            end_of_the_game(newsockfd, sockfd, user_fd_1);
                            break;
                            }
                    }
                    send_result(result, newsockfd, sockfd, user_fd_1);
                    while((len_of_mess = read(user_fd_2[0], message, MAX_SYM)) < 0) sleep(0.1);
                    is_continue = strcmp(message, "GameOver");
                    if(is_continue == 0) { 
                        num_of_bytes = my_write(newsockfd, "Lose Game", newsockfd, sockfd);
                        break;
                        }
                    }
                    break;
            }
        } else {
            if((newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen)) < 0) {
                printf("Accept stage has a problem");
                close(sockfd);
                exit(1);
            }
            while((num_of_bytes = read(newsockfd, line, MAX_SYM + 2)) > 0) {
                map_constructing(newsockfd, sockfd, line, 2);
                units = battle_preparing(newsockfd, sockfd, user_fd_2, user_fd_1);
                FILE* f2 = fopen("f2.txt", "r");
                FILE* f1 = fopen("f1.txt", "r");
                while(1) {
                    while((len_of_mess = read(user_fd_1[0], message, MAX_SYM)) < 0) sleep(0.1);
                    is_continue = strcmp(message, "GameOver");
                    if(is_continue == 0) {
                        num_of_bytes = my_write(newsockfd, "Lose Game", newsockfd, sockfd);
                        break;
                    }
                    coord = get_coordinates(newsockfd, arr_str, arr_col, &length);
                    result = hit_or_miss(coord, f1);
                    if(result == 1){
                        the_num_of_dead++;
                        if(the_num_of_dead == units) { 
                            end_of_the_game(newsockfd, sockfd, user_fd_2);
                            break;
                        }
                    }
                    send_result(result, newsockfd, sockfd, user_fd_2);
                }
                    break;
            }
        }
    }
    close(newsockfd);
    if(message != NULL) free(message);
    if(line != NULL) free(line);
    if(arr_str != NULL) free(arr_str);
    if(arr_col != NULL) free(arr_col);
    if(coord != NULL) free(coord);
}  

