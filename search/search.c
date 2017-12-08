#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <string.h>

#define MAX_SIZE 100




void search(const char *string, int deep, const char *file) {
    int counter = 0;
	DIR * papka;
	struct dirent *enter;
	counter++;
	printf("Find %s in directory %s\n", file, string);
	if(counter <= deep) { 
		if((papka = opendir(string)) == NULL) {
			perror("Can't opendir");
			exit(1);
		}
		while((enter = readdir(papka)) != NULL) {
			printf("%ld - %s [%d] %d\n", enter->d_ino, enter->d_name, enter->d_type, enter->d_reclen);
			if(!strcmp(enter->d_name, file)) {
				printf("I found  %s in directory %s\n", file, string);
				exit(EXIT_SUCCESS);    
			}
			if (enter->d_type == 4) {
				char new_dir[MAX_SIZE];
				strcpy(new_dir, string);
				strcat(new_dir, "/");
				strcat(new_dir, enter->d_name); 
				search(new_dir, deep, file);
			}
		}
		closedir(papka);
	}
    counter--;
}


int main(int argc, char** argv) {
	if(argc != 4) {
		printf("Usage: %s directory search_of_depth name\n", argv[0]);
		exit(1); 
	}
	int deep = atoi(argv[2]);
	char directory[MAX_SIZE];
	char file[MAX_SIZE];
	strcpy(directory, argv[1]);
	strcpy(file, argv[3]);
	search(directory, deep, file);
	return 0;
}
