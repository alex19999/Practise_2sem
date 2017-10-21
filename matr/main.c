#include"matrix.h"



int main(int argc, char** argv) {
    int time1 = clock();
	int counter = 0;
    char answer;
    int m_size = 0;
    if(atoi(argv[1]) != 0) m_size = atoi(argv[1]);
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
    int time2 = clock();
	printf("time = %d\n", time2-time1);
    return 0;
}
