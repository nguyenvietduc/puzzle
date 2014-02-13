// Write a program to find the first occurence of duplicate char in a char array
#include <stdio.h>

/*
Thought process:
- scan through char array
- hash table may work
- keep track of occurence
- hash table has O(1) lookup
- range of char: ASCII or Unicode
- made simple, let's say ASCII
- could have an array of 128 bins (ASCII is 7-bit encoding)
- set all bins to false
- first true encounter is the char to return
- convert from char to int for free from C
*/

#define SIZE 128

char first_dup_char(char* s) {
	char ret;
	int* occur;
	int idx;

	occur = (int*)malloc(sizeof(int) * SIZE);
	for (idx=0; idx<SIZE; ++idx)
		occur[idx] = 0;

	while (*s){
		ret = *s++;
		idx = (int)ret;
		if (occur[idx])
			return ret;
		occur[idx] = 1;
	}

	return ret;
}

int main() {
	char* char_array = "ths is some char array";
	char c;

	c = first_dup_char(char_array);
	printf("%c\n", c);
	return 0;
}