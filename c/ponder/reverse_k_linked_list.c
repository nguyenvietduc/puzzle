// Reverse every k-element in a linked list
#include <stdio.h>

/*
Thought process:
- singly linked list? assume so
- assume list starts at index 1
- examples:
	1 2 3 4 5 6 7
	k = 2
	output = 1 6 3 4 5 2 7

	1 2 3 4 5 6 7 8 9 0
	k = 3
	output = 1 2 9 4 5 6 7 8 3 0

- scan through list
- count until k
- if we have tail pointer, we can iterate from both ends,
then swap (caveat, must know the length of the list)
- how do we normally reverse a linked list?
-- by creating a new head
-- iterating over the list and then prepending the node to
head of new list
- can we leaverage the same?
-- every elements after k-th will be in front of the k-th

Here is the deal

1 2 3 .. k-1 k k+1 .. 2k-1 2k 2k+1 2k+2

1 2 3 .. k-1 2k+1 k+1 .. 2k-1 k 2k+1 2K+2

- hmm wouldn't work
- how about:
-- iterate through each element
-- reverse every k-th element in a separate list
-- iterate again to re-attach the elements
- well, this would be horribly slow

*/

int main() {
	return 0;
}