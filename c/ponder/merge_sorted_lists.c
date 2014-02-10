// Given two sorted linked list, return the linked list which is the merged list
// of the first two and also is sorted
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
	Merge two sorted linked lists without using extra node.
	Recursive function.
*/
node* merge(node* m, node* n) {
	if (!m)
		return n;

	if (!n)
		return m;

	// compare the heads of two lists
	if (m->data < n->data) {
		m->next = merge(m->next,n);
		return m;
	} else {
		n->next = merge(m,n->next);
		return n;
	}
}

/*
	Merge two linked lists. NOTE: This function modifies the lists in place.
*/
node* merge_with_node(node* m, node* n) {
	node* ret;
	if (m->data < n->data) {
		ret = m;
		m = m->next;
	}
	else {
		ret = n;
		n = n->next;
	}

	while (1) {
		if (!m) {
			ret->next = n;
			break;
		} else {
			ret->next = m;
			break;
		}
			
		if (m->data < n->data) {
			ret->next = m;
			m = m->next;
		} else {
			ret->next = n;
			n = n->next;
		}

		ret = ret->next;
	}

	return ret;

}

int main() {

	node * list = (node*)malloc(sizeof(node));
	node * list2 = (node*)malloc(sizeof(node));

	if (!list || !list2)
		return -1;

	list->data = -11;
	list->next = NULL;
	append(list,3);
	append(list,11);
	append(list,20);
	append(list,40);

	list2->data = 2;
	list2->next = NULL;
	append(list2,10);
	append(list2,14);

	// Merge two sorted lists
	node* merged_list;
	merged_list = merge(list, list2);
	print_list(merged_list);

	// Merge using extra node (and modifying the original lists)
	node* merged_list_2;
	merged_list_2 = merge_with_node(list, list2);
	print_list(merged_list_2);

	// Set the content of those two lists after the modifying merge
	print_list(list);
	print_list(list2);	

}