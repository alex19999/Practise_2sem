#ifndef _MATRIX_H_
#define _MATRIX_H_

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<time.h>

typedef struct data {
	int str_num; // номер текущей строки //
	int str_size; // размер  строки //
	float** matrix_a;
	float** matrix_b; // матрицы //
	float** res;
} pthread_data;

float myrandom(float min, float max); // мой персональный рандом //

void* thread_func(void* thread_data); // перемножение матриц //

void inizial_matrix_yourself(float** matr_a, float** matr_b, int size); // самостоятельное заполнение матриц //

void random_inizial(float** matr_a, float** matr_b, int size); // рандомное генерирование матриц //

void show_matrix(float** matrix, int size); // вывод матриц //

#endif /* _MATRIX_H_ */
