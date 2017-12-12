#ifndef _SEABATTLE_H_
#define _SEABATTLE_H_

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
#define CLIENT_1 1
#define CLIENT_2 2


int my_write(int descriptor, char* message, int newsockfd, int sockfd);// Just something more than simple function write.

int my_read(int descriptor, char* destiny, int max_length, int newsockfd, int sockfd);// The same.

void map_constructing(int newsockfd, int sockfd, char* line, int who);// This function reads strings from newsockfd and writes them into the needed file(parametr who is given, so we know the right file), so it is constructing map part.

int search_map(FILE* f); //Searches file and find units in it.

int check(int* coord, int* arr_1, int* arr_2, int length);// Checks whether competitors used these coordinates previously or not.

int hit_or_miss(int* coord, FILE* f);//Kill someone or not.

int check_map(FILE* f, int newsockfd, int sockfd);//Checks, how competitors constructed their maps(stops the game, if something is wrong).

int battle_preparing(int newsockfd, int sockfd, int* user_1, int* user_2);//Prepares users for killing each other:opens maps, cheks the number of units(calls search_map). Through this function competitors inform each other about their readiness. 

int* get_coordinates(int newsockfd, int* arr_str, int* arr_col, int* length);// Get coordinates and work with it.

void send_result(int result,int newsockfd, int sockfd, int* user_fd);//Send result about the shoot to the user and inform enemy about user's readiness to his shoot.

void end_of_the_game(int newsockfd, int sockfd, int* user_fd); //Tell users about end of the game, if it's the case.




#endif /* _SEABATTLE_H_ */

