/* Cholesky decomposition.
 * Compile as follows: gcc -g -Wall -o chol chol.c chol_gold.c -lpthread -lm -std=c99
 */

#define _XOPEN_SOURCE 600
/*
Apparently this preprocessor definition of _XOPEN_SOURCE is important since
without it, the barrier prototypes are not defined in pthread.h
*/

// includes, system
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>
#include "chol.h"
#include <pthread.h>

////////////////////////////////////////////////////////////////////////////////
// declarations, forward

Matrix allocate_matrix(int num_rows, int num_columns, int init);
int perform_simple_check(const Matrix M);
void print_matrix(const Matrix M);
extern Matrix create_positive_definite_matrix(unsigned int, unsigned int);
extern int chol_gold(const Matrix, Matrix);
void chol_using_pthreads(const Matrix, Matrix, int);
int check_results(float *, float *, unsigned int, float);

////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) 
{	
	// Check command line arguments
	// NOTE I modified this arguments line a bit for ease of running experiments
	if(argc != 3){
		printf("Error. Usage: ./chol <num_threads> <single_thread_enabled>\n");
		exit(0);
	}

	int num_threads = atoi(argv[1]);
	int single_thread_enabled = atoi(argv[2]); // 1 means to run the serial version
	 
	// Matrices for the program
	Matrix A; // The N x N input matrix
	Matrix reference; // The upper triangular matrix computed by the reference code
	Matrix U_using_pthreads; // The upper triangular matrix computed by the multi-threaded version
	
	// Initialize the random number generator with a seed value 
	srand(time(NULL));

	// Create the positive definite matrix. May require a few tries if we are unlucky
	int success = 0;
	while(!success){
		A = create_positive_definite_matrix(MATRIX_SIZE, MATRIX_SIZE);
		if(A.elements != NULL)
				  success = 1;
	}
	// print_matrix(A);
	// getchar();

	reference  = allocate_matrix(MATRIX_SIZE, MATRIX_SIZE, 0); // Create a matrix to store the CPU result
	U_using_pthreads =  allocate_matrix(MATRIX_SIZE, MATRIX_SIZE, 0); // Create a matrix to store the device result

	// compute the Cholesky decomposition using the refernce code
	struct timeval start, stop;	
	gettimeofday(&start, NULL);

	printf("Performing Cholesky decomposition on the CPU. \n");
	int status;
	if (single_thread_enabled == 1)
		status = chol_gold(A, reference);
	else
		status = 1;

	gettimeofday(&stop, NULL);
	printf("CPU run time = %0.2f s. \n", (float)(stop.tv_sec - start.tv_sec + (stop.tv_usec - start.tv_usec)/(float)1000000));
	if(status == 0){
			  printf("Cholesky decomposition failed. The input matrix is not positive definite. \n");
			  exit(0);
	}
	printf("Cholesky decomposition on the CPU was successful. \n");

	// Perform the Cholesky decomposition using the multi-threaded version.
	//The resulting upper triangular matrix should be returned in U_using_pthreads
	gettimeofday(&start, NULL);
	chol_using_pthreads(A, U_using_pthreads, num_threads);
	gettimeofday(&stop, NULL);
	printf("Pthreads: CPU run time= %0.2f s. \n", (float)(stop.tv_sec - start.tv_sec + (stop.tv_usec - start.tv_usec)/(float)1000000));

	// Check if the pthread result is equivalent to the expected solution..
	unsigned int size = reference.num_rows * reference.num_columns;
	int res = check_results(reference.elements, U_using_pthreads.elements, size, 0.01f);
	printf("Test %s\n", (1 == res) ? "PASSED" : "FAILED");

	// Free host matrices
	free(A.elements); 	
	free(U_using_pthreads.elements);	
	free(reference.elements); 
	return 1;
}

/**
	struct of argument passed to pthread function
*/
typedef struct pthread_args_struct {
	int thread_id;
	Matrix* A_p;
	Matrix* U_p;
	pthread_barrier_t* barrier;
	int num_threads;
} pthread_args_struct;

// Helper function to wait on barrier, since this is used quite often
void barrier_wait_func(pthread_barrier_t* barrier, int thread_id) {
	int rc = pthread_barrier_wait(barrier);
	if (rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD) {
		printf("™DEBUG] Thread %d could not wait on barrier\n", thread_id);
		exit(-1);
	}
}

// Helper function to determine range of operation
void range_determination_func(
	int begin,			/* begin index of the divided range */
	int end,			/* end index of the divided range */
	int num_threads,	/* number of threads sharing the range */
	int thread_id,		/* ID of thread invoking this function */
	int* start,			/* start index for this thread */
	int* range 			/* end index (exclusive) for this thread */)			
{
	// range size
	int total = end - begin + 1;
	// normal chunk size for each thread
	int chunk = total / num_threads;

	if (total < num_threads) {
		// if there are more threads than resource,
		// then no need to parallelize
		if (thread_id == (num_threads-1)) {
			*start = begin;
			*range = end+1;
		} else {
			*start = end;
			*range = end;
		}
	} else {
		// if there are more resource
		// parallelization makes sense
		*start = begin + thread_id * chunk;
		if (thread_id == (num_threads-1))
			*range = end + 1;
		else
			*range = (*start) + chunk;
	}
}

// Utility function to perform Cholesky decomposition
void* pthread_cholesky_func(void* f_arg) {
	pthread_args_struct* arg = (pthread_args_struct*)f_arg;

	// Populate arguments for this thread
	int thread_id = arg->thread_id;
	Matrix* A_p = arg->A_p;
	Matrix* U_p = arg->U_p;
	pthread_barrier_t* barrier = arg->barrier;
	int num_threads = arg->num_threads;

	// Utility variables
	int col_t, col_range;
	int row_t, row_range;
	unsigned int num_rows = A_p->num_rows;
	unsigned int num_cols = A_p->num_columns;
	// unsigned int mat_size = num_rows * num_cols;

	// Parallel copy content of A_p to U_p
	range_determination_func(0, num_rows-1, num_threads, thread_id, &row_t, &row_range);
	barrier_wait_func(barrier, thread_id);
	for (int i=row_t; i<row_range; ++i)
		for (int j=0; j<num_cols; ++j)
			U_p->elements[i*num_cols+j] = A_p->elements[i*num_cols+j];

	// Wait on barrier for all threads to be ready
	barrier_wait_func(barrier, thread_id);

	// Cholesky decomposition
	for (int k=0; k < num_rows; ++k) {
		// determine column range for division
		range_determination_func(k+1, num_cols-1, num_threads, thread_id, &col_t, &col_range);

		// Only thread 0 does square root
		if (thread_id == 0) {
			U_p->elements[k*num_cols + k] = sqrt(U_p->elements[k*num_cols + k]);
		}
		barrier_wait_func(barrier, thread_id);

		// Parallel division
		// WONDER: Is it faster to have thread 0 also do division?
		for (int j=col_t; j<col_range; ++j)
			U_p->elements[k*num_cols + j] /= U_p->elements[k*num_cols + k];
		barrier_wait_func(barrier, thread_id);

		// Naive parallelization: by rows, not very balanced work among threads
		// TODO load balance across all threads
		range_determination_func(k+1, num_rows-1, num_threads, thread_id, &row_t, &row_range);
		for (int i=row_t; i<row_range; ++i) {
			for (int j=i; j<num_rows; ++j)
				U_p->elements[i*num_cols+j] -= U_p->elements[k*num_cols+i]*U_p->elements[k*num_cols+j];
		}
		barrier_wait_func(barrier, thread_id);

	}

	barrier_wait_func(barrier, thread_id);

	// Zero out the lower triangular portion of U
	range_determination_func(0, num_rows-1, num_threads, thread_id, &row_t, &row_range);
	for (int i=row_t; i<row_range; ++i)
		for (int j=0; j<i; ++j)
			U_p->elements[i*num_cols+j] = 0.0;
}

/* Perform Cholesky decopmposition using pthreads. */
void chol_using_pthreads(const Matrix A, Matrix U, int num_threads)
{
	int i;

	// Pool of threads and their arguments
	pthread_t threads[num_threads];
	pthread_args_struct thread_args[num_threads];
	// TODO reuse arg instead
	// pthread_args_struct *arg;

	// Barrier init
	pthread_barrier_t barrier;
	pthread_barrier_init(&barrier, NULL, num_threads);

	// Create and launch threads
	for (i=0; i<num_threads; ++i) {
		// create argument list
		thread_args[i].A_p = &A;
		thread_args[i].U_p = &U;
		thread_args[i].thread_id = i;
		thread_args[i].barrier = &barrier;
		thread_args[i].num_threads = num_threads;

		// create thread
		pthread_create(&threads[i], NULL, pthread_cholesky_func, &thread_args[i]);
	}
	// Join threads
	for (i=0; i<num_threads; ++i)
		pthread_join(threads[i], NULL);
}


/* Function checks if the results generated by the single threaded and multi threaded versions match. */
int check_results(float *A, float *B, unsigned int size, float tolerance)
{
	for(int i = 0; i < size; i++)
		if(fabsf(A[i] - B[i]) > tolerance)
			return 0;
	return 1;
}

// Allocate a matrix of dimensions height*width
//	If init == 0, initialize to all zeroes.  
//	If init == 1, perform random initialization.
Matrix allocate_matrix(int num_rows, int num_columns, int init)
{
    	Matrix M;
    	M.num_columns = M.pitch = num_columns;
    	M.num_rows = num_rows;
    	int size = M.num_rows * M.num_columns;
		
	M.elements = (float *) malloc(size * sizeof(float));
	for(unsigned int i = 0; i < size; i++){
		if(init == 0) M.elements[i] = 0; 
		else
			M.elements[i] = (float)rand()/(float)RAND_MAX;
	}
    return M;
}	



