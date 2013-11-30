// ----------------
// Linked List in C
// ----------------

#include <stdio.h>

typedef struct IntElement {
  struct IntElement *next;
  int data;
} IntElement;

int insertInFront(IntElement **head, int data) {
  IntElement *newElem = (IntElement *)malloc(sizeof(IntElement));
  if (!newElem)
    return 0;

  newElem->data = data;
  newElem->next = *head;
  *head = newElem;
  return 1;
}

int printList(IntElement *head) {
  while(head) {
    printf("%d ", head->data);
    head = head->next;
  }
  printf("\n");
  return 1;
}

int freeList(IntElement *head) {
  IntElement *tmp;
  while(head) {
    tmp = head;
    free(tmp);
    head = head->next;
  }
}

IntElement *createNode(int data) {
  IntElement *node;
  if (!(node = (IntElement*)malloc(sizeof(IntElement))))
    return NULL;
  node->data = data;
  node->next = NULL;
  return node;
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
  
  // Return memory to the OS
  freeList(head);
}
