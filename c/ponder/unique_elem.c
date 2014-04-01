// Given an array of integers. Each number, except one, appears exactly twice.
// The remaining number appears exactly once.
// Write a program to return this unique number.
#include <stdio.h>

int unique(int* array) {
	int ret = 0;
	while (*array)
		ret = ret ^ *array++;
	return ret;
}

int main() {
	int array[] = {1,4,2,1,3,4,2};
	printf("%d\n", unique(array));
	return 0;
}