#ifndef _WASH_WIPE_H_
#define _WASH_WIPE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "split.h"

#define MAX_SYM 15
#define NUM_OF_ARG 2
#define TABLE_LIMIT 2

void* func_wash(void* fd_1); // функция, отвечающая за симуляцию мытья посуды;  

void* func_wipe(void* fd_2); // функция, отвечающая за симуляцию протирания посуды;

#endif /* _WASH_WIPE_H_ */



