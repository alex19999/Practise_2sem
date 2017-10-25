#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<unistd.h>
#include"split.h"
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_SYM_STR 100
#define MAX_ARG 10
#define MAX_SYM 10
#define MAX_OPER 25

// fixit: лишние пробелы

/*
ф-и надо называть так (и в целом разбивать на ф-и), что не смотря в тело ф-и
было ясно, что она делает. 
иногда чтобы не писать слишком длинное название, для ясности добавляют комментарий.

из названия "помочь системному вызовы execvp" мало что понятно
*/
void help_execvp(FILE* f_in) {
	pid_t pid;
	char* func_name;
	char* separator = " ";
	char* string_of_comands;
	char** tokens;
	int* quantity;
	int time_delay = 0;
	int offset = 0;
	int* status;
	int i = 0;
	tokens = (char**)calloc(MAX_OPER, sizeof(char*));
	quantity = (int*)calloc(MAX_ARG, sizeof(int));
	while(!feof(f_in)){
		string_of_comands = (char*)calloc(MAX_SYM_STR, sizeof(char));	
		fgets(string_of_comands, MAX_SYM_STR, f_in);
		string_of_comands[strlen(string_of_comands) - 1] = 0;
		Split(string_of_comands, separator, tokens, quantity);
		time_delay = atoi(tokens[0]);
		if(time_delay != 0) {
			func_name = tokens[1];
			offset = 1;
		} else {
			func_name = tokens[0];
			offset = 0;
			}
		pid = fork();
		/*
		fixit:
		сейчас код написан так, что если у вас i-я задача работает очень долго,
		то вы пропустите момент запуска i+1-й
		*/
		if(pid == 0) {
			sleep(time_delay);
			execvp(func_name, tokens + offset);
			exit(0);
		} else wait(status);
		free(string_of_comands);
	}
	free(tokens);

}
		
int main(int argc, char** argv) {
	/*
	fixit: название файла со списком задач как раз из аргументов командной строки и 
	надо было получить
	*/
	FILE* f_in = fopen("file_with_func.txt", "r");
	switch(argc - 1) {
		case 1 :
			/*
			fixit: что это за махинации с argv[1]?
			причем после них вы argv[1] и не используете ...
			*/
			argv[1] = "file_with_func.txt";
			break;
		default :
			printf("There are a bad number of arguments\n");
			break;
	}
	help_execvp(f_in);
return 0;
}
	
		
