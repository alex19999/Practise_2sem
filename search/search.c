#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <string.h>

#define MAX_SIZE 100
#define MAX_SYM 10

void decrease(char* string) {
    int len = 0;
	if(string == NULL) return;
	len = strlen(string);
    while(len != 0 && string [len] != '/') {
        string [len] = '\0';
        len--;
    }
    if(len == 0) return;
    string[len] = 0;
	return;
}

char* increase(char* string, char* extra_dir) {
    char* new_dir;
    new_dir = (char*)calloc(MAX_SIZE, sizeof(char));
	strcpy(new_dir, string);
	strcat(new_dir, "/");
	strcat(new_dir, extra_dir);
	return new_dir;
}


int search(char *string, int deep, const char *file) {
	DIR* dir;
    char* new_dir;
	struct dirent *enter;
    int result;
    int deep_ = deep;
    new_dir = (char*)calloc(MAX_SIZE, sizeof(char));
    if(deep < 0) return -1;
	printf("Find %s in directory %s\n", file, string);
	if((dir = opendir(string)) == NULL) {
		perror("Can't open directory");
		exit(1);
	}
	while((enter = readdir(dir)) != NULL) {
		if(!strcmp(enter->d_name, file)) {
            return 1;
        }
    }
    rewinddir(dir);
    while((enter = readdir(dir)) != NULL) {
		if(enter->d_type == 4 && strncmp(".", enter -> d_name, 1) && strncmp("..", enter -> d_name, 2)) {
		    new_dir = increase(string, enter->d_name);
            result = search(new_dir, deep - 1, file);
            if(result == 1) return 1;
            else if(result == -1) 
            	decrease(string);
            else return 0;
            }
	}
    free(new_dir);
    return -1;
}

//give directory, deep and needed file

int main(int argc, char** argv) {
    if(argc - 1 == 3) { 
	        int deep = atoi(argv[2]);
	        char* directory;
	        char* file;
            int res = 0; 
            directory = (char*)calloc(MAX_SIZE, sizeof(char));
            file = (char*)calloc(MAX_SIZE, sizeof(char));
            strcpy(directory, argv[1]);
            strcpy(file, argv[3]);
            res = search(directory, deep, file);
            printf("res = %d\n", res);
            free(directory);
            free(file);
    } else {
            printf("Incorect number of arguments\n");
    }
}
