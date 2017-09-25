#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<unistd.h>

#define MAX_SYM_STR 100
#define MAX_ARG 10
#define MAX_SYM 10



void help_execvp(FILE* f_in) {
	pid_t pid;
	char* func_name;
	char* separator = " ";
	char* string_of_comands;
	char** tokens;
	int* quantity;
	int time_delay = 0;
	int offset = 0;
	tokens = (char**)calloc(1, sizeof(char*));
	quantity = (int*)calloc(MAX_ARG, sizeof(int));
	while(!feof(f_in)){
		string_of_comands = (char*)calloc(MAX_SYM_STR, sizeof(char));	
		fgets(string_of_comands, MAX_SYM_STR, f_in);
		string_of_comands[strlen(string_of_comands) - 1] = 0;
		Split(string_of_comands, separator, tokens, quantity);
		time_delay = atoi(tokens[0]);
		pid = fork();
		if(pid == 0) {
		if(time_delay != 0) {
			func_name = tokens[1];
			offset = 1;
		} else {
			func_name = tokens[0];
			offset = 0;
			}
		//pid = fork();
		//if(pid == 0) {
			printf("delay = %d\n",time_delay);
			printf("offset = %d\n",offset);
			printf("func = %s\n",func_name);
			sleep(time_delay);
			execvp(func_name, tokens + offset);
			exit(0);
		}
		free(string_of_comands);
	}
	free(tokens);

}
		

int main(int argc, char** argv) {
	FILE* f_in = fopen("file_with_func.txt", "r");
	switch(argc - 1) {
		case 1 :
			argv[1] = "file_with_func.txt";
			break;
		default :
			printf("There are a bad number of arguments\n");
			break;
	}
	help_execvp(f_in);
return 0;
}
	
		
