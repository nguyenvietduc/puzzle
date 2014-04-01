// MPI_Scatter() example. Block distribution.
//
// $ mpicc -g -Wall -std=c99 -o gather_mpi gather_mpi.c
// $ mpiexec -n 4 ./gather_mpi
// ~~~~
#include <stdio.h>
#include <mpi.h>

void get_input_vector(double[],int,int,char[],int,MPI_Comm);
void print_vector(double[],int,int,char[],int,MPI_Comm);

int main() {
	int my_rank, comm_sz, n = 8, local_n;
	char vec_name[] = "A";

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	local_n = n / comm_sz;
	double local_a[local_n];

	// Get input
	get_input_vector(local_a, local_n, n, vec_name, my_rank, MPI_COMM_WORLD);

	// some computation on a thread's local chunk
	for (int i=0; i<local_n; ++i)
		local_a[i] *= 2;

	// MPI_Barrier(MPI_COMM_WORLD);

	// gather all data to thread 0
	print_vector(local_a, local_n, n, vec_name, my_rank, MPI_COMM_WORLD);

	

	MPI_Finalize();
	return 0;
}

void get_input_vector(double local_a[], int local_n, int n, char vec_name[], int my_rank, MPI_Comm comm) {
	double* a = NULL;
	int i;

	if (my_rank == 0) {
		a = (double*)malloc(n*sizeof(double));
		printf("Enter the vector %s of size %d\n", vec_name, n);
		for (i=0; i<n; ++i)
			scanf("%lf", &a[i]);
		MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);
		free(a);
	} else {
		MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);
	}
}

void print_vector(double local_a[], int local_n, int n, char vec_name[], int my_rank, MPI_Comm comm) {
	double* a = NULL;
	int i;

	if (my_rank == 0) {
		a = (double*)malloc(n*sizeof(double));
		MPI_Gather(local_a,local_n, MPI_DOUBLE, a, local_n, MPI_DOUBLE, 0, comm);
		printf("%s\n", vec_name);
		for (i=0; i<n; ++i)
			printf("%f ", a[i]);
		printf("\n");
		free(a);
	} else {
		MPI_Gather(local_a, local_n, MPI_DOUBLE, a, local_n, MPI_DOUBLE, 0, comm);
	}
}