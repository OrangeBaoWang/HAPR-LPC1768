// Created by Oliver Lea 17/01/2014

#include "stdlib.h"

#include "debug.h"
#include "filter.h"
#include "filterQueue.h"

FilterNode *currentNode;
FilterNode *root;

// Need to add way for node to contain filter parameters
int enqueue(Filter *newFilter) {

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
	currentNode->filter = newFilter;

	return 0;
}

int dequeue(Filter *targetFilter) {

	currentNode = root;

	if (currentNode->next == 0) {
		return -1;
	}

	// While not at the end of the list, compare the filter
	// structs for equivalence.
	// Will return -1 if no equivalent filter struct found
	while (currentNode->next != 0) {
		if  ((((currentNode->next)->filter)->filterFunction) == 
				targetFilter->filterFunction) {

			if ((((currentNode->next)->filter)->parameters[0]) ==
					targetFilter->parameters[0]) {

				if ((((currentNode->next)->filter)->parameters[1]) ==
						targetFilter->parameters[1]) {

					currentNode->next = (currentNode->next)->next;
					return 0;
				}
			}
		}
		currentNode = currentNode->next;
	}

	return -1;

}

// Will not increment the currentNode if already at the end
// of the list
void incrementCurrentNode(void) {

	if (currentNode->next != 0) {
		currentNode = currentNode->next;
	}
}

int queue_init(void) {

	// root now points to a node struct
	root = malloc(sizeof(FilterNode));

	if (root == NULL) {
		WARN(0, "Malloc failed for filterQueue");
		return -1;
	}

	// The node pointer points to null
	root->next = 0;

	currentNode = root;

	return 0;
}
