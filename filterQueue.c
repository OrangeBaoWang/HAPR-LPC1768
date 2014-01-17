#include "stdlib.h"

#include "debug.h"

typedef struct FilterNode {
		int filterID;
		struct FilterNode *next;
} FilterNode;

FilterNode *currentNode;
FilterNode *root;

int addFilter(int filter) {

	// Ensure the currentNode is at the end of the linked list
	if (currentNode != 0) {
		while (currentNode->next != 0) {
			currentNode = currentNode->next;
		}
	}

	// Creates a new node at the end of the linked list
	currentNode->next = malloc(sizeof(FilterNode));
	// Moves the currentNode pointer to the new node
	currentNode = currentNode->next;

	if (currentNode == 0) {
		debugThrow("Out of memory when allocating new node to filter queue");
		return -1;
	}

	// Initialize the new node
	currentNode->next = 0;
	currentNode->filterID = filter;
}

int queueInit() {

	// root now points to a node struct
	root = malloc(sizeof(FilterNode));
	// The node pointer points to null
	root->next = 0;

	currentNode = root;
}
