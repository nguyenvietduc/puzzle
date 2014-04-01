// MPI_Allgather() example
// Matrix-vector multiplication
//
// $ mpicc -g -Wall -std=c99 -o allgather_mat_mult allgather_mat_mult.c
// $ mpiexec -n 4 ./allgather_mat_mult
// ~~~~
#include <stdio.h>
#include <mpi.h>

int main() {
	int my_rank, comm_sz;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	MPI_Finalize();
	return 0;
}

void mat_vect_mult(
	double local_A[],
	double local_x[],
	double local_y[],
	int	local_m,
	int	n,
	int	local_n,
	MPI_Comm comm) {

	double* x;
	int local_i, j;
	int local_ok = 1;

	x = (double*)malloc(n*sizeof(double));
	MPI_Allgather(local_x, local_n, MPI_DOUBLE, x, local_n, MPI_DOUBLE, comm);

	for (local_i=0; local_i<local_m; local_i++) {
		local_y[local_i] = 0.0;
		for (j=0; j<n; j++)
			local_y[local_i] += local_A[local_i*n+j] * x[j];
	}

	free(x);
}