// Given two sorted linked list, return the linked list which is the merged list
// of the first two and also is sorted
#include <stdio.h>

typedef struct node {
	int data;
	struct node* next;
} node;

void print_list(node *head)
{
	node* current = head;
	while(current)
	{
		printf("%d ", current->data);
		current = current->next;
	}
	printf("\n");
}

void append(node *head, int val)
{
	node* current = head;
	while (current->next)
		current = current->next;

	current->next = malloc(sizeof(node));
	current->next->data = val;
	current->next->next = NULL;
}

void merge_no_node(node* m, node* n) {

}

void merge(node* m, node* n) {
	
}

int main() {

	node * list = (node*)malloc(sizeof(node));

	if (!list)
		return -1;

	list->data = 1;
	list->next = NULL;

	print_list(list);
	append(list,2);
	append(list,3);
	append(list,4);
	append(list,5);
	print_list(list);

}