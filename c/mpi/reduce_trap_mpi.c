// The trapezoidal rule in MPI
// Using collective communication and proper I/O
//
// $ mpicc -g -Wall -std=c99 -o reduce_trap_mpi reduce_trap_mpi.c
// $ mpiexec -n 4 ./reduce_trap_mpi
// ~~~~
#include <stdio.h>
#include <mpi.h>

double f(double);
double Trap(double,double,int,double);
void get_input(int my_rank,int,double*,double*,int*);

int main() {
	int my_rank, comm_sz, n, local_n;
	double a, b, h, local_a, local_b;
	double local_int, total_int;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	// Get input
	get_input(my_rank, comm_sz, &a, &b, &n);

	// TODO local_n may be different for one particular thread if n % comm_sz != 0
	h = (b-a)/n;
	local_n = n/comm_sz;

	local_a = a+my_rank*local_n*h;
	local_b = local_a+local_n*n;
	local_int = Trap(local_a,local_b,local_n,h);

	// Using collective communication
	MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (my_rank == 0) {
		printf("With n = %d trapezoids, our estimate\n", n);
		printf("of the integral from %f to %f = %.15e\n", a, b, total_int);
	}

	MPI_Finalize();
	return 0;
}

void get_input(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p) {
	int dest;

	if (my_rank == 0) {
		// only thread 0 scan for input form stdin

		printf("Enter a,b, and n\n");
		scanf("%lf %lf %d", a_p, b_p, n_p);
		for (dest=1; dest<comm_sz; ++dest) {
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

double Trap(double left_endpt, double right_endpt, int trap_count, double base_len) {
	double est, x;
	int i;

	est = (f(left_endpt) + f(right_endpt))/2.0;
	for (i=1; i<trap_count; ++i) {
		x = left_endpt + i*base_len;
		est += f(x);
	}

	est = est*base_len;

	return est;
}

double f(double x) {
	return (x*x - x + 1);
}