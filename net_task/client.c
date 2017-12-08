#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>
#include"split.h"

#define MAX_SYM 10
#define MAX_HIT 20
#define MAX_SHOOTS 100
#define MAP_SIZE 10

void show_previous_strikes(int* str, int* col, char** results, int length) {
    int counter = length;
    while(length >= 0) {
        printf("coordinates(%d, %d), result:%s\n", str[length], col[length], results[counter--]);
        length--;
    }
}

void main(int argc, char** argv) {
    int sockfd;
    int n;
    int i;
    int j;
    char** tokens;
    char* sendline;
    char* recvline;
    char* delimeter = " ";
    int* quantity;
    int* str_shoots;
    int* col_shoots;
    char** my_shoots;
    int length = 0;
    struct sockaddr_in servaddr;
    str_shoots = (int*)calloc(MAX_SHOOTS, sizeof(int));
    col_shoots = (int*)calloc(MAX_SHOOTS, sizeof(int));
    tokens = (char**)calloc(2, sizeof(char*));
    my_shoots = (char**)calloc(MAX_SHOOTS, sizeof(char*));
    for(j = 0; j++; j < 2) {
        tokens[j] = (char*)calloc(MAX_SYM, sizeof(char));
    }
    for(j = 0; j++; j < MAX_SHOOTS) {
        my_shoots[j] = (char*)calloc(MAX_SYM, sizeof(char));
    }
    sendline = (char*)calloc(MAX_SYM, sizeof(char));
    recvline = (char*)calloc(MAX_SYM, sizeof(char));
    quantity = (int*)calloc(1, sizeof(int));
    if(argc != 2) {
        printf("Usage: a.out <IP address>\n");
        exit(1);
    }
    bzero(sendline, 20);
    bzero(recvline, 20);
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket stage has a problem");
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51277);
    if(inet_aton(argv[1], &servaddr.sin_addr) == 0) {
        printf("Invalid IP address\n");
        exit(1);
    }
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connect stage has a problem");
        exit(1);
    }
    printf("Hello to our nem game. The rules are simple:\n");
    printf("you will have some ships, like your enemy(default:20 units, but this number can be increase or decrease(the main idea you must at least the same number as your enemy(server will check it))).\n");
    printf("By turns you will strike in each other, programm will give your statistics. Now you must create your map\n"); 
    printf("* is block\n");
    printf("o is unit.\n"); 
    printf("After your enemy finish this level game begins. ATTENTION: If you give some coordinates and nothing is happen, don't worry, it just means, that now your enemy strikes. GOOD LUCK, HAVE FUN\n");
    for(i = 0; i < 10; i++) {
        printf("Line[%d]:", i + 1);
        fflush(stdin);
        fgets(sendline, 1000, stdin);
        fflush(stdin);
        if((n = write(sockfd, sendline, strlen(sendline) + 1)) < 0) {
            perror("Can't right");
            close(sockfd);
            exit(1);
        }
    }
    while(( n = read(sockfd, recvline, 999)) < 0) { 
        sleep(0.1);
    }
    printf("Enemy is ready = %s\n", recvline);
    while(1) {
        printf("Give me coordinates\n");
        fflush(stdin);
        fgets(sendline, 1000, stdin);
        Split(sendline, delimeter, tokens, quantity);
        str_shoots[length] = atoi(tokens[0]);
        col_shoots[length] = atoi(tokens[1]);
        n = write(sockfd, sendline, strlen(sendline));
        if(( n = read(sockfd, recvline, 999)) < 0) {
            perror(NULL);
            close(sockfd);
            exit(1);
        }
        my_shoots[length] = recvline;
        printf("my_shoots[length] = %s\n", my_shoots[length]);
        show_previous_strikes(str_shoots, col_shoots, my_shoots, length);
        length++;
        printf("\n*result* = %s\n", recvline);
        if(strcmp(recvline, "Lose") == 0) { 
            printf("result:%s\n", recvline);
            break;
        }
        if(strcmp(recvline, "Victory") == 0) {
            printf("result:%s\n", recvline);
            break;
        }
    } 
    printf("Game Over\n");
    //free(quantity);
    //free(tokens);
    //free(sendline);
    //free(recvline);
    //free(my_shoots);
    //free(str_shoots);
    //free(col_shoots);
    close(sockfd);
}
