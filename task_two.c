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
	tokens = (char**)calloc(1, sizeof(char*));
	quantity = (int*)calloc(MAX_ARG, sizeof(int));
	while(!feof(f_in)){
		string_of_comands = (char*)calloc(MAX_SYM_STR, sizeof(char));	
		fgets(string_of_comands, MAX_SYM_STR, f_in);
		string_of_comands[strlen(string_of_comands) - 1] = 0;
		Split(string_of_comands, separator, tokens, quantity);
		func_name = tokens[0];
		pid = fork();
		if(pid == 0) {
			execvp(func_name, tokens);
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
	
		
