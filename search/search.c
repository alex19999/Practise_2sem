#include "search.h"

void decrease(char* string) { //decreases our string-path
    int len = 0;
    if(string == NULL) return;
    len = strlen(string);
    while(len != 0 && string [len] != '/') {
        string [len] = '\0';
        len--;
    }
    /*
    кажется, что если использовать do/while, то не надо будет дублировать следующие 2 строки
    */
    if(len == 0) return;
    string[len] = 0;
    return;
}

char* increase(char* string, char* extra_dir) { //increases our string-path
    char* new_dir;
    new_dir = (char*)calloc(MAX_SIZE, sizeof(char));
    strcpy(new_dir, string);
    strcat(new_dir, "/");
    strcat(new_dir, extra_dir);
    return new_dir;
}

/*
используйте вместо -1, 1
enum SearchingResult
{
  Found,
  NotFound
};
*/
int search(char *string, int deep, const char *file) {
    DIR* dir;
    char* new_dir;
    struct dirent *enter;
    int result;
    new_dir = (char*)calloc(MAX_SIZE, sizeof(char));
    if(deep < 0) {
        return -1;
    }
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
	/* почему не strcmp, если захардкоженные константы передаются определенной длины ? */
        if(enter->d_type == 4 && strncmp(".", enter -> d_name, 1) && strncmp("..", enter -> d_name, 2)) {
            new_dir = increase(string, enter->d_name);
            result = search(new_dir, deep - 1, file);
            if(result == 1) { 
		/*
		fixit: По-хорошему ваша ф-я не должна ничего писать на экран ... от нее требуется найти файл и записать путь до
		него в оговоренное место, либо оповестить, что не удалось это сделать.
		Вывод найденного пути должен осуществляться вне ф-и search
		*/
                printf("File %s was founded in %s directory\n", file, new_dir);
                return 1;
            }   
            if(result == -1) {
                decrease(string);
            }
        }
    }
    free(new_dir);
    /*
    fixit: у вас память утекает ... число вызовов calloc и free не совпадает
    */
    printf("File was not founded\n");
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
        search(directory, deep, file);
	/*
	вот здесь нужно проверить, нашли мы файл или нет и вывесте соответствующий текст на экран
	*/
        free(directory);
        free(file);
    } else {
            printf("Incorect number of arguments\n");
    }
}
