#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"split.h"
#include<malloc.h>


#define MAX_SYM 1000

void help_execvp(FILE* f_in,char* divisor) {
	char* func_name;
	char* string_of_comands;
	char** array_of_arg;	
	int* quantity;
	char sym;
	int counter = 0;
	f_in = fopen("file_with_func.txt","r");
	array_of_arg = (char**)calloc(1,sizeof(char*));
	while(!feof(f_in)){
		string_of_comands = (char*)calloc(MAX_SYM,sizeof(char));	
		fgets(string_of_comands,MAX_SYM,f_in);
		printf("str_of_comands = %s\n",string_of_comands);
		Split(string_of_comands,divisor,array_of_arg,quantity);
		printf("hale\n");	
		func_name = array_of_arg[0];
		printf("%s\n",func_name);
		execvp(func_name,array_of_arg);
		
	}
}
		

int main(int argc,char** argv) {
	FILE* f_in;
	int* result = NULL;
	char* divisor;	
	divisor = (char*)calloc(1,sizeof(char));
	divisor = " ";
	printf("divisor = %s\n",divisor);
	switch(argc - 1) {
		case 1 :
			argv[1] = "file_with_func.txt";
			break;
		default :
			printf("There are a bad number of arguments\n");
			break;
	}
	help_execvp(f_in,divisor);
return 0;
}
	
		
