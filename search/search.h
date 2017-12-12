#ifndef _SEARCH_H_
#define _SEARCH_H_

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <string.h>

#define MAX_SIZE 100
#define MAX_SYM 10


int search(char* string, int depth, const char* file);

/*
не особо ясные названия ф-й: возможно, changeToParentDir, append яснее выражают ваши намерения
*/
void decrease(char* string);
char* increase(char* string, char* extra_dir);

#endif /* _SEARCH_H_ */
