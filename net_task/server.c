#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"split.h"
#include<ctype.h>

#define MAX_SYM 100
#define CORRECT_NUM_OF_BRICKS 100
#define MAP_SIZE 100

int my_write(int descriptor, char* message, int length,int newsockfd, int sockfd) {
    int num_of_bytes;
    if((num_of_bytes = write(descriptor, message, length)) < 0) {
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

int check(int coord_1, int coord_2, int* arr_1, int* arr_2, int length) {
    int iter = 0;
    for(iter = 0; iter < length; iter++) {
        if(arr_1[iter] == coord_1) {
            if(arr_2[iter] == coord_2) {
                return 0;
            }
        }
    }
    return 1;
}




int hit_or_miss(int coord_1, int coord_2, FILE* f){
    char alive = 'o';
    char suspect;
    int offset = 0;
    if(coord_1 > 0 && coord_1 <= 10){
        if(coord_2 > 0 && coord_2 <= 10){
            offset = (coord_1 - 1)*10 + coord_2 + coord_1 - 2;
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

void main() {
    int sockfd;
    int newsockfd;
    int clilen;
    int length = 0;
    int res = 0;
    int result = 0;
    int i, n ,k;
    int user_fd_1[2];
    int user_fd_2[2];
    int decision_fd[2];
    int counter_1 = 0;
    int counter_2 = 0;
    int units = 0;
    int enemy = 0;
    int num_of_bytes = 0;
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    char* recvline = "end";
    char* message;
    char* line;
    int* arr_str;
    int* arr_col;
    int len_of_mess_to;
    int len_of_mess_into;
    int the_num_of_str = 0;
    int the_num_of_col = 0;
    int the_num_of_dead_1 = 0;
    int the_num_of_dead_2 = 0;
    int is_continue = 0;
    int fd_1 = open("f1.txt", O_WRONLY);
    int fd_2 = open("f2.txt", O_WRONLY);
    char** tokens;
    int* quantity;
    char* delimeter = " ";
    pid_t pid;
    line = (char*)calloc(MAX_SYM, sizeof(char));
    arr_col = (int*)calloc(MAX_SYM, sizeof(int));
    arr_str = (int*)calloc(MAX_SYM, sizeof(int));
    quantity = (int*)calloc(1, sizeof(int));
    tokens = (char**)calloc(2, sizeof(char*));
    message = (char*)calloc(MAX_SYM, sizeof(char));
    for(i = 0; i < 2; i++) {
        tokens[i] = (char*)calloc(2, sizeof(char));
    }
    if(pipe(user_fd_1) < 0 || pipe(user_fd_2) || pipe(decision_fd) < 0) {
        perror("Can't create pipe");
        exit(1);
    }
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket stage has a problem");
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51277);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
        perror("Bind stage has a problem");
        close(sockfd);
        exit(1);
    }
    if(listen(sockfd, 5) < 0) {
        perror("Listen stage hs a problem");
        close(sockfd);
        exit(1);
    }
    pid = fork();
    while(1) {
        clilen = sizeof(cliaddr);
        if(pid == 0) {
            if((newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen)) < 0) {
                perror("Accept stage has a problem");
                close(sockfd);
                exit(1);
            }
            while((num_of_bytes = read(newsockfd, line, MAX_SYM)) > 0){
                num_of_bytes = my_write(fd_1, line, strlen(line), newsockfd, sockfd);
                counter_1++;
                if(counter_1 > 9) {
                    close(user_fd_1[0]);
                    close(user_fd_2[1]);
                    len_of_mess_to = my_write(user_fd_1[1], "ready", MAX_SYM, newsockfd, sockfd);
                    while(len_of_mess_into = read(user_fd_2[0], message, MAX_SYM) < 0) sleep(0.1);
                    close(fd_1);
                    FILE* f1 = fopen("f1.txt", "r");
                    FILE* f2 = fopen("f2.txt", "r");
                    num_of_bytes = my_write(newsockfd, "ready", MAX_SYM, newsockfd, sockfd);
                    check_map(f1, newsockfd, sockfd);
                    enemy = search_map(f2);
                    units = search_map(f1);
                    if(units != enemy) {
                        perror("Cheating, game will stoped");
                        my_write(newsockfd, "end", MAX_SYM, newsockfd, sockfd);
                        close(sockfd);
                        close(newsockfd);
                    }
                    while(1) {
                        while((num_of_bytes = read(newsockfd, line, MAX_SYM)) < 0) sleep(0.1);
                        Split(line, delimeter, tokens, quantity);
                        the_num_of_str = atoi(tokens[0]);
                        the_num_of_col = atoi(tokens[1]);
                        res = check(the_num_of_str, the_num_of_col, arr_str, arr_col, length);
                        arr_col[length] = the_num_of_col;
                        arr_str[length] = the_num_of_str;
                        length++;
                        if(res == 1) {
                            result = hit_or_miss(the_num_of_str, the_num_of_col, f2);
                            if(result == 1) {
                                the_num_of_dead_1++;
                                if(the_num_of_dead_1 == units) {
                                    len_of_mess_to = my_write(user_fd_1[1], "end", MAX_SYM, newsockfd, sockfd);
                                    num_of_bytes = my_write(newsockfd, "Victory", MAX_SYM, newsockfd, sockfd);
                                    break;
                                }
                                num_of_bytes = my_write(newsockfd, "Hitting", MAX_SYM, newsockfd, sockfd);
                            } else {
                                num_of_bytes = my_write(newsockfd, "Miss", MAX_SYM, newsockfd, sockfd);
                            }
                        } else {
                            num_of_bytes = my_write(newsockfd, "Miss", MAX_SYM, newsockfd, sockfd);
                        }
                        len_of_mess_to = my_write(user_fd_1[1], "shoot", MAX_SYM, newsockfd, sockfd);
                        while((len_of_mess_into = read(user_fd_2[0], message, MAX_SYM)) < 0) sleep(0.1);
                        is_continue = strcmp(message, "end");
                        if(is_continue == 0) { 
                                num_of_bytes = my_write(newsockfd, "Lose", MAX_SYM, newsockfd, sockfd);
                                break;
                        }
                    }
                    break;
                }
            }
        } else {
            if((newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen)) < 0) {
                perror("Accept stage has a problem");
                close(sockfd);
                exit(1);
            }
            while((num_of_bytes = read(newsockfd, line, MAX_SYM)) > 0){
                num_of_bytes = my_write(fd_2, line, strlen(line), newsockfd, sockfd);
                counter_2++;
                if(counter_2 > 9) {
                    close(fd_2);
                    close(user_fd_2[0]);
                    close(user_fd_1[1]);
                    len_of_mess_to = my_write(user_fd_2[1], "ready", MAX_SYM, newsockfd, sockfd);
                    while((len_of_mess_into = read(user_fd_1[0], message, MAX_SYM)) < 0) sleep(0.1);
                    FILE* f2 = fopen("f2.txt", "r");
                    FILE* f1 = fopen("f1.txt", "r");
                    num_of_bytes = my_write(newsockfd, "ready", MAX_SYM, newsockfd, sockfd);
                    check_map(f2, newsockfd, sockfd);
                    units = search_map(f1);
                    enemy = search_map(f2);
                    if(enemy != units) {
                        perror("Cheating, game will be stopped");
                        n = my_write(newsockfd, "end", MAX_SYM, newsockfd, sockfd);
                        close(sockfd);
                        close(newsockfd);
                    }
                    while(1) {
                        while((len_of_mess_into = read(user_fd_1[0], message, MAX_SYM)) < 0) sleep(0.1);
                        is_continue = strcmp(message, "end");
                        if(is_continue == 0) {
                            num_of_bytes = my_write(newsockfd, "Lose", MAX_SYM, newsockfd, sockfd);
                            break;
                        }
                        while((num_of_bytes = read(newsockfd, line, MAX_SYM)) < 0) sleep(0.1);
                        Split(line, delimeter, tokens, quantity);
                        the_num_of_str = atoi(tokens[0]);
                        the_num_of_col = atoi(tokens[1]);
			            res = check(the_num_of_str, the_num_of_col, arr_str, arr_col, length);
                        arr_col[length] = the_num_of_col;
                        arr_str[length] = the_num_of_str;
                        length++;
			            if(res == 1) {
                        	result = hit_or_miss(the_num_of_str, the_num_of_col, f1);
                        	if(result == 1){
                            	the_num_of_dead_2++;
                            	if(the_num_of_dead_2 == units) { 
                                	len_of_mess_to = my_write(user_fd_2[1], "end", MAX_SYM, newsockfd, sockfd);
                                	num_of_bytes = my_write(newsockfd, "Victory", MAX_SYM, newsockfd, sockfd);
                                	break;
                            	}
                            	num_of_bytes = my_write(newsockfd, "Hitting", MAX_SYM, newsockfd, sockfd);
                        	} else {
                            	num_of_bytes = my_write(newsockfd, "Miss", MAX_SYM, newsockfd, sockfd);
                        	}
			            } else {
				            num_of_bytes = my_write(newsockfd, "Miss", MAX_SYM, newsockfd, sockfd);
			            }
                        len_of_mess_to = my_write(user_fd_2[1], "shoot", MAX_SYM, newsockfd, sockfd);
                    }
                    break;
                }
            }
        }
    }
        if(n < 0) {
            perror("n < 0");
            close(sockfd);
            close(newsockfd);
            exit(1);
        }
        close(newsockfd);
}  

