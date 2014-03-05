// Naive IO with MPI
// What will happen if all threads do I/O in MPI?
//
// $ mpicc -g -Wall -std=c99 -o naive_io_mpi naive_io_mpi.c
// $ mpiexec -n <num_threads> ./naive_io_mpi
// ~~~~~
#include <stdio.h>
#include <mpi.h>

int main() {
	int my_rank, comm_sz;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	printf("Proc %d of %d > Does anyone see this message?\n", my_rank, comm_sz);

	MPI_Finalize();
	return 0;
}
/*
	One will notice that there is interleaving among those printed line.
	Try to run with a large number of thread (8, for example) a few times.

	Notice the indeterministic result? That is because of the contention among those
	threads on stdout. It is not possible to predict the outcome as the OS takes care
	of scheduling.

	Thus, normally, we have one single thread do all I/O related, such as geting input
	and wrting output to stdout.
*/