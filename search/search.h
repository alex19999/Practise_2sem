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


enum SearchingResult search(char* string, int depth, const char* file, char* answer);

void changeToParentDir(char* string);

void append(char* string, char* extra_dir);

#endif /* _SEARCH_H_ */
