// Write a program to convert a sorted linked list to a balanced BST
#include <stdio.h>

typedef struct l_node {
	int value;
	struct node* next;
} node;

void insert(node* head, int data) {
	node* current = head;
	while(current->next && current->value < data)
		current = current->next;

	if (!current) {

	}
}

int main() {

}