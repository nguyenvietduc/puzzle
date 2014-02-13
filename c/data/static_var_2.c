// Static variables
/*

*/
#include <stdio.h>

char** func1_str();
char** func2_str();

int main() {
	char** ptr1 = NULL;
	char** ptr2 = NULL;

	ptr1 = func1_str();
	printf("\n [%s] \n", *ptr1);

	ptr2 = func2_str();
	printf("\n [%s] \n", *ptr2);

	printf("\n [%s] \n", *ptr1);

}

char** func1_str() {
	char* p = "Linux";
	return &p;
}

char** func2_str() {
	char* p = "Windows";
	return &p;
}