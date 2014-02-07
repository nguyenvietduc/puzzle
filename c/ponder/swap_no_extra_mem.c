#include <stdio.h>

int getIndex(int idx, int N) {
	return (idx % 3) * N + (idx / 3);
}

void swap(int *x, int *y) {
	if (x != y) {
		*x ^= *y;
		*y ^= *x;
		*x ^= *y;
	}
}

void orderArray(int a[], int len) {
	int N = len / 3;
	for(int i = 0; i < len; i++) {\
		int idx = getIndex(i, N);
		while(idx < i) {
			idx = getIndex(idx, N);
		}
		printf("i: %d; new_idx: %d\n", i, idx);
		swap(&a[i], &a[idx]);
	}
}

int main () {
	int len = 24;
	int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
	for(int i = 0; i < len; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");
	orderArray(a, len);
	for(int i = 0; i < len; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");
	return 0;
}