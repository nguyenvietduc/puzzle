/*
Given a binary tree, print all root-to-leaf paths
*/
#include <stdio.h>
#include <stdlib.h>

struct node {
	int data;
	struct node* left;
	struct node* right;
};

void printPathsRecur(struct node* node, int path[], int pathLen);
void printArray(int array[], int len);

void printPaths(struct node* node) {
	int path[1000];
	printPathsRecur(node, path, 0);
}

void printPathsRecur(struct node* node, int path[], int pathLen) {
	if (!node)
		return;
	path[pathLen++] = node->data;
	if (!node->left && !node->right)
		printArray(path,pathLen);
	else {
		printPathsRecur(node->left,path,pathLen);
		printPathsRecur(node->right,path,pathLen);
	}
}

void printArray(int array[], int len) {
	int i;
	for (i=0; i<len; ++i)
		printf("%d ", array[i]);
	printf("\n");
}

struct node* newnode(int data) {
	struct node* node = (struct node*)malloc(sizeof(struct node));
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	return node;
}

int main() {
	/* Constructed binary tree is
			10
		  /   \
		8		2
	  /  \	   /
    3     5  2
	*/
	struct node* root = newnode(10);
	root->left        = newnode(8);
	root->right       = newnode(2);
	root->left->left  = newnode(3);
	root->left->right = newnode(5);
	root->right->left = newnode(2);
	printPaths(root);
	return 0;
}