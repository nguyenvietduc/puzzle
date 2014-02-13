// Reverse a linked list in groups of given size
#include <stdio.h>

// define struct for linked list node
typedef struct node {
	int data;
	struct node* next;
} node;

/*
	Print utility function for printing linked list to stdout
*/
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

/*
	Add a new node to end of linked list
*/
void append(node *head, int val)
{
	node* current = head;
	while (current->next)
		current = current->next;

	current->next = (node*)malloc(sizeof(node));
	current->next->data = val;
	current->next->next = NULL;
}

/*

Examples:
input: 	1->2->3->4->5->6->7->8->NULL and k = 3 
output: 3->2->1->6->5->4->8->7->NULL

input: 	1->2->3->4->5->6->7->8->NULL and k = 5
output:	5->4->3->2->1->8->7->6->NULL

Thought process:
- similar to reservsing the entire list
- keep two pointers: head and tail
- prepeding if (index % k) > 0
- reset head to tail then index hits to k-th

*/

node* reverse_k(node* list, int k) {
	node* current = list;
	node* head = NULL, *tail = NULL;
	int idx = 0;

	if (!list) return NULL;

	// reverse the first k nodes
	while (current && idx < k) {
		tail = current->next;
		current->next = head;
		head = current;
		current = tail;
		idx++;
	}

	// list is the old head of the list;
	// tail points to the (k+1) node
	// recursively reverse the tail
	list->next = reverse_k(tail, k);

	return head;
}

int main() {
	node* list = (node*)malloc(sizeof(node));
	list->data = 1;
	list->next = NULL;

	append(list,2);
	append(list,3);
	append(list,4);
	append(list,5);
	append(list,6);
	append(list,7);
	append(list,8);

	print_list(list);

	list = reverse_k(list, 5);
	print_list(list);	

	return 0;
}