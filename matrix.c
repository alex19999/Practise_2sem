#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>

int N = 0;
int M = 0;

typedef struct data {
	int str_num; // номер текущей строки //
	int str_size; // размер  строки //
	float** matrix_a;
	float** matrix_b; // матрицы //
	float** res;
} pthread_data;

float myrandom(float min, float max) {
    return (float)(rand())/RAND_MAX*(max - min) + min;
}

// перемножение матриц //

void* thread_func(void* thread_data) { 
	pthread_data* data = (pthread_data*)thread_data;
	int i = 0;
    int j = 0;
    int k = 0;
    float sum = 0;
	for(i = 0; i < M; i++) {
        for(k = 0; k < M; k++) {
		    for( j = 0; j < N; j++) {
		        sum = sum + data->matrix_a[i][j] * data->matrix_b[j][k];
                data->res[i][k] = sum;
		    }
        sum = 0;
        }
    }	 
}

// самостоятельное задание матриц // 

void inizial_matrix_yourself(float** matr_a, float** matr_b, int size) {
	int i = 0;
	int j = 0;
	printf("Please, give me %d strings of your matrix_a with %d elements in each other(using enter)\n", size, size);
	for(i = 0; i < size; i++) {
		for(j = 0; j < size; j++) {
			scanf("%f", &matr_a[i][j]);
		}
	}
	printf("Please, do the same to matrix_b\n");
	for(i = 0; i < size; i++) {
		for(j = 0; j < size; j++) {
			scanf("%f", &matr_b[i][j]);
		}
	}
}

// рандомное генерирование матриц //

void random_inizial(float** matr_a, float** matr_b, int size) {
    srand(time(NULL));
    int i = 0;
    int j = 0;
    float min = 0;
    float max = 0;
    printf("Please, give me a range of random(using space)\n");
    scanf("%f %f", &min, &max);
    for( i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            matr_a[i][j] = myrandom(min, max);
            matr_b[i][j] = myrandom(min, max);
        }
    }
}

// вывод матриц //

void show_matrix(float** matrix, int size) {
	int i = 0;
	int j = 0;
	for (i = 0; i < size; i++) {
		for(j = 0; j < size ; j++) {
			if(j == size - 1) printf("%f*\n", matrix[i][j]);
            else printf("%f ", matrix[i][j]);
		}
	}
}



int main(int argc, char** argv) {
	int counter = 0;
    char answer;
    int m_size = 0;
    if(atoi(argv[1]) != 0) m_size = atoi(argv[1]);
    N = m_size;
    M = m_size;
	float** matrix1 = (float**)calloc(m_size, sizeof(float*));
	float** matrix2 = (float**)calloc(m_size, sizeof(float*));
	float** res_matrix = (float**)calloc(m_size, sizeof(float*));
	for(counter = 0; counter < m_size; counter++) {
		matrix1[counter] = (float*)calloc(m_size, sizeof(float));
		matrix2[counter] = (float*)calloc(m_size, sizeof(float));
		res_matrix[counter] = (float*)calloc(m_size, sizeof(float));
	}
						 
	pthread_t* threads = (pthread_t*)calloc(m_size, sizeof(pthread_t));
	pthread_data* thr_data = (pthread_data*)calloc(m_size, sizeof(pthread_data));
    
    printf("Do you want to fill matrics yourself?(use y or n)\n");
    scanf("%c", &answer);
    if(answer == 'y') 
    inizial_matrix_yourself(matrix1, matrix2, m_size);
    else random_inizial(matrix1, matrix2, m_size);
    printf("Matrix_a \n******************\n");
	show_matrix(matrix1, m_size);
    printf("Matrix_b \n******************\n");
	show_matrix(matrix2, m_size);
    printf("Result \n******************\n");

	for(counter = 0; counter < m_size; counter++){
		thr_data[counter].str_num = counter;
		thr_data[counter].str_size = m_size;
		thr_data[counter].matrix_a = matrix1;
		thr_data[counter].matrix_b = matrix2;
		thr_data[counter].res = res_matrix;
		
		pthread_create(&(threads[counter]), NULL, thread_func, &thr_data[counter]);
	}		
	for(counter = 0; counter < m_size; counter++) {
        pthread_join(threads[counter], NULL);
    }
    show_matrix(res_matrix, m_size);
    printf("******************\n");
	free(threads);
	free(thr_data);
	
	for(counter = 0; counter < m_size; counter++) {
		free(matrix1[counter]);
		free(matrix2[counter]);
		free(res_matrix[counter]);
	}

	free(matrix1);
	free(matrix2);
	free(res_matrix);
	return 0;
}
