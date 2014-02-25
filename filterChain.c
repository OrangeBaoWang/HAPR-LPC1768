// Created by Oliver Lea 17/01/2014

#include "lpc_types.h"

#include "stdlib.h"

#include "global.h"
#include "filter.h"
#include "filterChain.h"
#include "debug.h"
#include "filters/envFollower.h"

FilterNode *root;

// The 32-bit container the sample is put into while filtering
// takes place
uint32_t dSample;

void freeNode(FilterNode *nodeToFree) {

	if ((nodeToFree->filter)->sfilter == NULL) {
		free(((nodeToFree->filter)->pfilter)->filterOne);
		free(((nodeToFree->filter)->pfilter)->filterTwo);
		free((nodeToFree->filter)->pfilter);
	} else {
		free((nodeToFree->filter)->sfilter);
	}

	free(nodeToFree->filter);
	free(nodeToFree);

	return;
}

void enqueue(Filter *newFilter) {

	FilterNode *currentNode = root;

	// Ensure the currentNode is at the end of the linked list
	while (currentNode->next != NULL) {
		currentNode = currentNode->next;
	}

	// Creates a new node at the end of the linked list
	currentNode->next = malloc(sizeof(FilterNode));
	// Moves the currentNode pointer to the new node
	currentNode = currentNode->next;

	// Test to check malloc did not fail
	// Will not continue executing after throw
	if (currentNode == NULL) {
		THROW("Out of memory when allocating new node to filter queue");
	}

	// Initialize the new node
	currentNode->next = NULL;
	currentNode->filter = newFilter;
}

// Returns 0 on success, -1 otherwise
int enqueueByIndex(Filter *newFilter, float index) {

	FilterNode *currentNode = root;

	uint16_t indexInt = index;
	uint16_t currentIndex = 1;

	while (currentNode->next != NULL) {

		if (currentIndex == indexInt) {

			FilterNode *tempNode = currentNode->next;

			currentNode->next = malloc(sizeof(FilterNode));
			currentNode = currentNode->next;

			currentNode->filter = newFilter;
			currentNode->next = tempNode;

			if (currentNode == NULL) {
				THROW("Out of memory when allocating new node to filter queue");
				return -1;
			}

			return 0;
		}
		currentNode = currentNode->next;
		currentIndex++;
	}

	// Otherwise, if the chain is empty
	enqueue(newFilter);
	
	return 0;
}

// Takes a filter as input and attempts to find a filter of the
// same parameters. If found, that filter will be dequeued and 0 returned
// Otherwise, -1 returned
int dequeue(Filter *targetFilter) {

	FilterNode *nodeToFree;
	FilterNode *currentNode = root;

	if (currentNode->next == NULL) {
		return -1;
	}

	if (targetFilter->pfilter == NULL) {

		while (currentNode->next != NULL) {

			if (((currentNode->next)->filter)->pfilter == NULL) {
				if (filterEq(targetFilter->sfilter,
						((currentNode->next)->filter)->sfilter) == 1) {

					nodeToFree = currentNode->next;
					currentNode->next = (currentNode->next)->next;

					freeNode(nodeToFree);
					return 0;
				}
			}
			currentNode = currentNode->next;
		}
	} else {
		// If the filter is a parallel filter
		while (currentNode->next != NULL) {
			if (((currentNode->next)->filter)->pfilter != NULL) {
				if (filterEq((targetFilter->pfilter)->filterOne,
						(((currentNode->next)->filter)->pfilter)->filterOne) == 1) {

					if (filterEq((targetFilter->pfilter)->filterTwo,
							(((currentNode->next)->filter)->pfilter)->filterTwo) == 1) {

						nodeToFree = currentNode->next;
						currentNode->next = (currentNode->next)->next;

						freeNode(nodeToFree);
						return 0;
					}
				}
			}
			currentNode = currentNode->next;
		}
	}	
	return -1;
}

// Will dequeue a filter from the filter chain based on its position in the chain
// Returns 0 on success, -1 otherwise
int dequeueByIndex(float index) {

	FilterNode *nodeToFree;
	FilterNode *currentNode = root;

	uint16_t indexInt = index;
	uint16_t currentIndex = 1;

	while (currentNode->next != NULL) {

		if (currentIndex == indexInt) {
			nodeToFree = currentNode->next;
			currentNode->next = (currentNode->next)->next;

			freeNode(nodeToFree);
			return 0;
		}
		currentNode = currentNode->next;
		currentIndex++;
	}
	return -1;
}

// Will empty the filter chain of all filterNodes
// Returns -1 if the chain is already empty, 0 otherwise
int dequeueAll(void) {

	FilterNode *nodeToFree;

	if (root->next == NULL) {
		return -1;
	}

	nodeToFree = root->next;

	while (root->next != NULL) {
		// If at the last filter in the chain, free the filter node and return
		if ((root->next)->next == NULL) {
				root->next = NULL;
				
				freeNode(nodeToFree);
				return 0;
		}
		// Not at the last filter in the chain - rearrange root to point to the next filter
		// after the one to free and then free the node no longer in the chain
		root->next = (root->next)->next;

		freeNode(nodeToFree);

		// nodeToFree becomes the first filterNode in the chain
		nodeToFree = root->next;
	}
	return 0;
}

// Will return 1 if two filters are equivalent, 0 otherwise
int filterEq(SFilter *targetFilter, SFilter *currentFilter) {

	if ((targetFilter->filterFunction) == (currentFilter->filterFunction)) {
		int i;
		for (i = 0; i < 5; i++) {
			if ((targetFilter->parameters[i]) == (currentFilter->parameters[i])) {
				continue;
			} else {
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

// Will apply all filters present in the filter chain
// If no filters in chain, simply return the given value
uint16_t applyFilters(uint16_t sample) {

	FilterNode *currentNode = root;

	if (currentNode->next == NULL) {
		return sample;
	}

	dSample = sample;

	// While not at the end of the list, apply each filter to
	// the value in dSample
	while(currentNode->next != NULL) {
		currentNode = currentNode->next;

		if ((currentNode->filter)->sfilter == NULL) {
			dSample = mixParallel((currentNode->filter)->pfilter,
						dSample);
		} else {
			dSample =
				(*(((currentNode->filter)->sfilter)->filterFunction))
						(dSample,
						((currentNode->filter)->sfilter));
		}
	}

	// If the value from the filters is greater than 2^12 (the
	// highest value of the ADC (defined as AMPLITUDE_MAX),
	// sample is clipped to 2^12
	if (dSample > AMPLITUDE_MAX) {
		sample = AMPLITUDE_MAX;
	} else {sample = dSample;}


	return (sample);
}

void printQueue(void) {

	FilterNode *currentNode = root;

	uint16_t index = 1;

	printToTerminal("\n\rFILTER CHAIN:\n\r\r");

	if (currentNode->next == NULL) {
		printToTerminal("Empty\n\r");
		return;
	}

	while (currentNode->next != NULL) {

		currentNode = currentNode->next;	

		printfToTerminal("%d\n\r", index++);

		if ((currentNode->filter)->sfilter != NULL) {
			(*(((currentNode->filter)->sfilter)->printFunction))
					((currentNode->filter)->sfilter);
		} else {
			printToTerminal("Parallel\n\r\t");

			(*((((currentNode->filter)->pfilter)->filterOne)->printFunction))
					(((currentNode->filter)->pfilter)->filterOne);

			printToTerminal("\t");
			(*((((currentNode->filter)->pfilter)->filterTwo)->printFunction))
					(((currentNode->filter)->pfilter)->filterTwo);
		}
	}
	return;
}

int chain_init(void) {

	// root now points to a node struct
	root = malloc(sizeof(FilterNode));

	if (root == NULL) {
		WARN(0, "Malloc failed for filterQueue");
		return -1;
	}

	// The node pointer points to null
	root->next = NULL;

	return 0;
}
