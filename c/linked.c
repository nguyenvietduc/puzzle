// ----------------
// Linked List in C
// ----------------

#include <stdio.h>

/*
   struct IntElement for singly linked list of integer
*/
typedef struct IntElement {
  struct IntElement *next;
  int data;
} IntElement;

/*
   Insert a value in front of a linked list
*/
int insertInFront(IntElement **head, int data) {
  IntElement *newElem = (IntElement *)malloc(sizeof(IntElement));
  if (!newElem)
    return 0;

  newElem->data = data;
  newElem->next = *head;
  *head = newElem;
  return 1;
}

/*
   Print a linked list
*/
int printList(IntElement *head) {
  while(head) {
    printf("%d ", head->data);
    head = head->next;
  }
  printf("\n");
  return 1;
}

/*
   Free a linked list
*/
int freeList(IntElement **head) {
  IntElement *tmp = *head;
  while(tmp) {
    IntElement *next = tmp->next;
    free(tmp);
    tmp = next;
  }
  *head = NULL;
  return 1;
}

/*
   Create a node
*/
IntElement *createNode(int data) {
  IntElement *node;
  if (!(node = (IntElement*)malloc(sizeof(IntElement))))
    return NULL;
  node->data = data;
  node->next = NULL;
  return node;
}

/*
   find a value in a linked list
*/
IntElement *find(IntElement *head, int data) {
  while (head && head->data != data)
    head = head->next;
  return head;
}

/*
   Delete a node in a linked list
*/
int deleteNode(IntElement **head, IntElement *node) {
  IntElement *elem = *head;
  if (node == *head) {
    *head = elem->next;
    free(node);
    return 1;
  }

  while(elem) {
    if (elem->next == node) {
      // elem is preceding node
      elem->next = node->next;
      free(node);
      return 1;
    }
    elem = elem->next;
  }

  return 0;
}

/*
   Remove duplication in linked list
*/
void removeDup(IntElement **head) {
  IntElement *elem = *head;
  IntElement *runner;

  while(elem) {
    runner = elem;
    while (runner->next) {
      if (elem->data == runner->next->data)
	runner->next = runner->next->next;
      else
	runner = runner->next;
    }
    elem = elem->next;
  }
}

main() {
  // Allocate new node
  IntElement *head = (IntElement *)malloc(sizeof(IntElement));
  head->data = 5;
  head->next = NULL;
  // Using helper function
  IntElement *node = createNode(2);

  head->next = node;
  printList(head);
  
  // Insert to head
  insertInFront(&head,4);
  printList(head);

  // Find an element in list
  IntElement *elem = find(head,2);
  if (elem)
    printf("%d\n", elem->data);
  else
    printf("Not found");

  printList(head);

  // delete a node
  IntElement *del = createNode(2);
  deleteNode(&head, del);
  printList(head);
  if (node)
    deleteNode(&head, node);
  else
    printf("node was freed");
  printList(head);

  insertInFront(&head, 3);
  insertInFront(&head, 2);
  insertInFront(&head, 5);
  insertInFront(&head, 5);
  insertInFront(&head, 2);
  insertInFront(&head, 7);
  insertInFront(&head, 3);
  insertInFront(&head, 5);
  insertInFront(&head, 5);
  insertInFront(&head, 1);
  insertInFront(&head, 2);
  insertInFront(&head, 7);
  printList(head);

  removeDup(&head);
  printList(head);
  
  // Return memory to the OS
  freeList(&head);
}
