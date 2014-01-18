#include "debug.h"

typedef struct FilterNode {
	int filterID;
	int parameters[3];
	struct FilterNode *next;
} FilterNode;

FilterNode *currentNode;
FilterNode *root;

// Need to add way for node to contain filter parameters
int enqueue(int filterType) {

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
		THROW("Out of memory when allocating new node to filter queue");
		return -1;
	}

	// Initialize the new node
	currentNode->next = 0;
	currentNode->filterID = filterType;

	return 0;
}

// Need way to find specific node (not just by filterID)
int dequeue(int filterType) {

	currentNode = root->next;

	// Find the node with filterID == filterType
	// If it does not exist, return -1
	while ((currentNode->filterID) != filterType) {
		currentNode = currentNode->next;
		if (currentNode->next == 0) {
			return -1;
		}
	}



}

int queueInit() {

	// root now points to a node struct
	root = malloc(sizeof(FilterNode));
	// The node pointer points to null
	root->next = 0;

	currentNode = root;
}
