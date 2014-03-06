// MPI_Bcast() example
//
// $ mpicc -g -Wall -std=c99 -o bcast_mpi bcast_mpi.c
// $ mpiexec -n 4 ./bcast_mpi
// ~~~~
#include <stdio.h>
#include <mpi.h>

void get_input_bcast(int,int,double*,double*,int*);

int main() {
	int my_rank, comm_sz, n;
	double a, b;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	// Get input
	get_input_bcast(my_rank, comm_sz, &a, &b, &n);

	printf("Thread %d of %d> I got %lf, %lf, %d.\n", my_rank, comm_sz, a, b, n);

	MPI_Finalize();
	return 0;
}

void get_input_bcast(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p) {
	if (my_rank == 0) {
		printf("Enter a, b, and n\n");
		scanf("%lf %lf %d", a_p, b_p, n_p);
	}

	MPI_Bcast(a_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(b_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(n_p, 1, MPI_INT, 0, MPI_COMM_WORLD);
}