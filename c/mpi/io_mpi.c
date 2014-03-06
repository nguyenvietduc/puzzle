// IO with MPI
//
// $ mpicc -g -Wall -std=c99 -o io_mpi io_mpi.c
// $ mpiexec -n <num_threads> ./io_mpi
// ~~~~~
#include <stdio.h>
#include <mpi.h>

void get_input(int,int,double*,double*,int*);

int main() {
	int my_rank, comm_sz;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	double a,b; int n;
	get_input(my_rank, comm_sz, &a, &b, &n);
	printf("Proc %d of %d > I got %lf, %lf, %d\n", my_rank, comm_sz, a, b, n);

	MPI_Finalize();
	return 0;
}

void get_input(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p) {
	int dest;

	if (my_rank == 0) {
		// should only let thread 0 handle input & output
		printf("Enter a, b, and n\n");
		scanf("%lf %lf %d", a_p, b_p, n_p);
		for (dest=1; dest<comm_sz; dest++) {
			MPI_Send(a_p, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
			MPI_Send(b_p, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
			MPI_Send(n_p, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
		}
	} else {
		MPI_Recv(a_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(b_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(n_p, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
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