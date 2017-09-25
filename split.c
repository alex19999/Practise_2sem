#include<stdio.h>
#include<stdlib.h>
#include"split.h"
#include<string.h>

#define MAX_SYM 1000
#define MAX_WORDS 100
#define MAX_SEP 100


void Split(char* str_in, char* separator, char** tokens, int* quantity) {
        int counter = 0;
        int iter = 0;
        char* istr;
        int length_of_token = 0;
        char* new_str;
	new_str = str_in;
        strcat(str_in, separator);
        if(str_in != NULL && separator != NULL) {
                for(iter == 0; iter <= strlen(str_in); iter++) {
                        istr = strstr(str_in, separator);
                        length_of_token = strlen(str_in) - strlen(istr);
                        if(istr == NULL) {
                                tokens[counter] = (char*)calloc(strlen(new_str), sizeof(char));
                                strcpy(tokens[counter], new_str);
                                break;
                        } else {
                                tokens[counter] = (char*)calloc(length_of_token, sizeof(char));
                                strncpy(tokens[counter], str_in, length_of_token);
                                str_in = istr + strlen(separator);

                        }
		counter++;
                }
        *quantity = counter;
        }
}
