// Write a program to convert sorted array to balanced binary search tree
#include <stdio.h>

typedef struct node {
	int value;
	struct node* left;
	struct node* right;
} node;

void print_tree_preorder(node* root) {
	if (!root)
		return;
	printf("%d ", root->value);
	print_tree_preorder(root->left);
	print_tree_preorder(root->right);
}

void print_tree_inorder(node* root) {
	if (!root)
		return;
	print_tree_preorder(root->left);
	printf("%d ", root->value);
	print_tree_preorder(root->right);
}

void print_tree_postorder(node* root) {
	if (!root)
		return;
	print_tree_preorder(root->left);
	print_tree_preorder(root->right);
	printf("%d ", root->value);
}

void sorted_array_to_balanced_bst(node** root, int array[], int left, int right) {
	if (left > right)
		return;
	*root = (node*)malloc(sizeof(node));
	int mid = left + (right - left) / 2;
	(*root)->value = array[mid];
	sorted_array_to_balanced_bst(&(*root)->left,array,left,mid-1);
	sorted_array_to_balanced_bst(&(*root)->right,array,mid+1, right);
}

int get_array_length(int *array) {
	return (sizeof(array)/sizeof(array[0]));
}

int main() {
	int array[] = { 9, 12, 14, 17, 19, 23, 50, 54, 67, 72, 76};
	node* root;
	int length = sizeof(array)/sizeof(array[0]);
	printf("%d\n", length);
	sorted_array_to_balanced_bst(&root, array, 0 , length-1);
	print_tree_preorder(root);
	return 0;
}