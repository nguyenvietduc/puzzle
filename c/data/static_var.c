// Static variables
/*
Life time of Static variables remains equal to the life time of the program.
*/
#include <stdio.h>

char** func_str();

int main() {
	char** ptr = NULL;
	ptr = func_str();
	printf("\n [%s] \n", *ptr);
	return 0;
}

char** func_str() {
	char* p = "Linux";
	return &p;
}