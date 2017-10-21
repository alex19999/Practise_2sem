#include"matrix.h"


float myrandom(float min, float max) {
    return (float)(rand())/RAND_MAX*(max - min) + min;
}



void* thread_func(void* thread_data) { 
	pthread_data* data = (pthread_data*)thread_data;
	int i = 0;
    int j = 0;
    int k = 0;
    float sum = 0;
	for(i = 0; i < data->str_size; i++) {
        for(k = 0; k < data->str_size; k++) {
		    for( j = 0; j < data->str_size; j++) {
		        sum = sum + data->matrix_a[i][j] * data->matrix_b[j][k];
                data->res[i][k] = sum;
		    }
        sum = 0;
        }
    }	 
}



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
