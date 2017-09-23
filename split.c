#include<stdio.h>
#include<stdlib.h>
#include"split.h"
#include<string.h>

#define MAX_SYM 1000
#define MAX_WORDS 100
#define MAX_DIVIDER 100


void Split(char* str_in,char* divider,char** tokens,int* quantity) {
        int counter = 0;
        int iter = 0;
        char* istr;
        int length_of_token = 0;
        tokens = (char**)calloc(MAX_WORDS,sizeof(char*));
        str_in[strlen(str_in) - 1] = 0;
        divider[strlen(divider) - 1] = 0;
        istr =(char*)calloc(MAX_DIVIDER,sizeof(char));
        char* new_str = str_in;
        strcat(str_in,divider);
        if(str_in != NULL || divider != NULL) {
                for(iter == 0; iter <= strlen(str_in); iter++) {
                        istr = strstr(str_in,divider);
                        length_of_token = strlen(str_in) - strlen(istr);
                        if(istr == NULL) {
                                tokens[counter] = (char*)calloc(strlen(new_str),sizeof(char));
                                strcpy(tokens[counter],new_str);
                                counter++;
                                break;
                        } else {
                                tokens[counter] = (char*)calloc(length_of_token,sizeof(char));
                                strncpy(tokens[counter],str_in,length_of_token);
                                counter++;
                                str_in = istr + strlen(divider);

                        }
                }
        *quantity = counter;
        printf("%d\n",*quantity);
        }
}
