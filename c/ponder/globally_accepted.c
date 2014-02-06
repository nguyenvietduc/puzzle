// Will globally declared variable point to a local variable
// and still represent the same memory location?
#include <stdio.h>
#include <stdlib.h>

int *p;

void locally_accepted(int*);
void globally_accepted();

int main() {
	int *n;
	n = (int*)malloc(sizeof(int)*4);
	n[0] = 1;
	n[1] = 2;
	n[2] = 3;
	n[3] = 4;
	locally_accepted(n);
	return 0;
}

void locally_accepted(int* n) {
	int* current = n;
	while(*current)
		printf("%d ", *current++);
	printf("\n");
}

void globally_accepted() {
	int* current = p;
	//while(*current++)
}