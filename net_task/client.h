#ifndef _CLIENT_H_
#define _CLIENT_H_

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

#define MAX_SYM 20
#define MAX_HIT 20
#define MAX_SHOOTS 100
#define MAP_SIZE 10

void show_previous_strikes(int* str, int* col, char** results, int length); //Shows our previous shoots

void print_map(int* str_shoots, int* col_shoots, char* results, int length); //Print shoots on map(h = Hit, m = Miss);


#endif /* _CLIENT_H_ */

