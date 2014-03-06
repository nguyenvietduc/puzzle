// MPI_Allreduce() function
// MPI_Reduce() function
// They all do reduction operation on the input data. However, the main
// difference is that, MPI_Allreduce() distributes the reduction result
// to all threads while MPI_Reduce() keeps the reduction at a single
// destination.
//
// $ mpicc -g -Wall -std=c99 -o all_reduce_mpi all_reduce_mpi.c
// $ mpiexec -n 4 ./all_reduce_mpi
// ~~~~
#include <stdio.h>
#include <mpi.h>

int main() {
	int my_rank, comm_sz, total;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	total = 0;
	// Reduce only
	MPI_Reduce(&my_rank, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	printf("[REDUCE] Thread %d of %d > I got %d\n", my_rank, comm_sz, total);

	MPI_Barrier(MPI_COMM_WORLD);

	// Allreduce
	total = 0;
	MPI_Allreduce(&my_rank, &total, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	printf("[ALLREDUCE] Thread %d of %d > I got %d\n", my_rank, comm_sz, total);	

	MPI_Finalize();
	return 0;
}