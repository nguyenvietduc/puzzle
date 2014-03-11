// Counting sort
#include <stdio.h>
#include <string.h>
#define RANGE 255

void count_sort(char*);

int main() {
	char str[] = "geeksforgeeks";
	count_sort(str);
	printf("Sorted string is %s\n", str);
	return 0;
}

// counting sort algorithm
void count_sort(char* str) {
	// sorting a char array in alphabetical order

	char output[strlen(str)];
	int count[RANGE+1], i;
	memset(count, 0, sizeof(count));

	// Store count of each character
	char* cur = str;
	while (*cur)
		++count[*cur++];

	for (i=1; i<=RANGE; ++i)
		count[i] += count[i-1];

	for (i=0; str[i]; ++i) {
		output[count[str[i]]-1] = str[i];
		--count[str[i]];
	}

	for (i=0; str[i]; ++i)
		str[i] = output[i];
}