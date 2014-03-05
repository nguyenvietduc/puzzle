// Naive scanf in MPI
// What will happen if all threads invoke scanf?
//
// $ mpicc -g -Wall -std=c99 -o naive_scanf_mpi naive_scanf_mpi.c
// $ mpiexec -n <num_threads> ./naive_scanf_mpi
// ~~~~~
#include <stdio.h>
#include <mpi.h>

int main() {
	int my_rank, comm_sz;
	int local_n = 10, local_m = 20, local_t = 30;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	printf("[Thread %d] Enter n, m, and t:\n", my_rank);
	scanf("%d %d %d", &local_n, &local_m, &local_t);
	printf("[Thread %d] n = %d, m = %d, t = %d\n", my_rank, local_n, local_m, local_t);

	MPI_Finalize();
	return 0;
}

/*
	Well, turn out when I ran on Ubuntu 12.04, only thread 0 get the inputs.
	Other threads pass the scanf() and print the original local values: 10, 20, 30.

	This could be internal implementation of MPI for my system. Or it could possibly
	be the case that if I ran enough number of times, results would be different.
*/