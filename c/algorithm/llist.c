// Everything linked list
#include <stdio.h>

typedef struct node {
	int val;
	struct node * next;
} node;

void print_list(node *head)
{
	node* current = head;
	while(current)
	{
		printf("%d ", current->val);
		current = current->next;
	}
	printf("\n");
}

void prepend(node **head, int n)
{
	node* tmp = (node*)malloc(sizeof(node));
	tmp->val = n;
	tmp->next = *head;
	*head = tmp;
}

void append(node *head, int val)
{
	node* current = head;
	while (current->next)
		current = current->next;

	current->next = malloc(sizeof(node));
	current->next->val = val;
	current->next->next = NULL;
}

int pop(node **head)
{
	int retval = -1;
	node* tmp = NULL;

	if (!(*head))
		return -1;

	tmp = (*head)->next;
	retval = (*head)->val;
	free(*head);
	*head = tmp;

	return retval;
}

int remove_by_index(node** head, int k)
{
	int retval = -1;
	node* current = *head;
	node* tmp = NULL;

	if (k==0)
		return pop(head);

	for (int i=0; i < k-1; i++)
	{
		if (!current->next) // index out of range
			return -1;
		current = current->next;
	}

	tmp = current->next;
	retval = tmp->val;
	current->next = tmp->next;
	free(tmp);

	return retval;
}

int main()
{
	node * list = (node*)malloc(sizeof(node));

	if (!list)
		return -1;

	list->val = 1;
	list->next = NULL;

	print_list(list);
	prepend(&list,2);
	print_list(list);
	append(list,3);
	append(list,4);
	append(list,5);
	print_list(list);
	pop(&list);
	print_list(list);
	remove_by_index(&list,3);
	print_list(list);

}