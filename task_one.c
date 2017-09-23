#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<unistd.h>
#include<string.h>
#include"split.h"
#define MAX_SYM 1000
#define MAX_WORDS 100
#define MAX_SEP 100

void Split(char* str_in, char* separator, char** tokens, int* quantity);

int main() {
	char* str_in;
	int cnt = 0;	
	int* quantity;
	char** tokens;
	char* separator;
	quantity = (int*)calloc(MAX_WORDS, sizeof(int));
        str_in = (char*)calloc(MAX_SYM, sizeof(char));
	printf("please, give me a string to divide it into tokens\n");
	fgets(str_in, MAX_SYM, stdin);
        printf("please, give me a symbol to divide\n");
	fgets(separator, MAX_WORDS, stdin);
	Split(str_in, separator, tokens, quantity);
return 0;
}



void Split(char* str_in, char* separator, char** tokens, int* quantity) {
        int counter = 0;
        int iter = 0;
	char* istr;
	int length_of_token = 0;
	tokens = (char**)calloc(MAX_WORDS, sizeof(char*));
	str_in[strlen(str_in) - 1] = 0; 
	separator[strlen(separator) - 1] = 0;
	istr =(char*)calloc(MAX_SEP, sizeof(char));
	char* new_str = str_in;
	strcat(str_in, separator);
	if(str_in != NULL && separator != NULL) {
		for(iter == 0; iter <= strlen(str_in); iter++) {
			istr = strstr(str_in, separator);
			length_of_token = strlen(str_in) - strlen(istr);
			if(istr == NULL) {
				tokens[counter] = (char*)calloc(strlen(new_str), sizeof(char));
				strcpy(tokens[counter], new_str);
				counter++;
				break;
			} else {
				tokens[counter] = (char*)calloc(length_of_token, sizeof(char));
				strncpy(tokens[counter],str_in, length_of_token);
				counter++;
				str_in = istr + strlen(separator);
				
			}
		}
        *quantity = counter;
	}
}
