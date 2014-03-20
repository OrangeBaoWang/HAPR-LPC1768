// Created by Oliver Lea 17/01/2014

// O-Edit: Added apply filters function that takes the current chain of filters
//	and the sample to be filtered, and returns the post-processed sample - 19/01/2014

// O-Edit: Added in support for parallel filters in the incrementCurrentNode()
//	function (subsequently removed) and the applyFilters() function - 21/01/2014

// O-Edit: Added dequeuing parallel filters to the dequeue() function. Also added a
//	filterEq() function that tests if two SFilters are equivalent - 23/01/2014

// O-Edit: Fixed a bug in the dequeue function that would cause segmentation
//	faults upon attempting to dequeue a filter - 24/01/2014

// O-Edit: Began work on a printQueue() function that would allow the filter chain
//	to be printed from the UI. Work in progress - 29/01/2014

// O-Edit: Modified filterEq(), applyFilters() and the (unfinished) printQueue()
//	functions to support a change in how filter parameters are passed (Used to be
//	passed as two floats, but are now passed as an array of five floats) - 31/01/2014

// O-Edit: Finished the printQueue() function, which makes use of the printFunction
//	function pointer that every instance of a filter possesses - 04/02/2014

// O-Edit: Parallel filter dequeueing and printing support added in - 06/02/2014

// O-Edit: dequeueByIndex() function added that removes a filter from the filter
//	chain according to its index - 12/02/2014

// O-Edit: enqueueByIndex() function added that adds a filter to the filter chain
//	in a specified position given by the index passed to the function - 17/02/2014

// O-Edit: Added freeNode() function (frees previously malloc'd space now unused by
//	old filters) and dequeueAll() function (empties the filter chain) - 18/02/2014

// O-Edit: Fixed freeNode() function (previous one caused seg faults as it was freeing
//	the structs in the incorrect order) and added calls to free the FilterNode
//	structs to all dequeue functions - 24/02/2014

// O-Edit: Added assertions that no malloc() call in filterChain.c returns NULL - 28/02/2014

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

	// Test to check malloc did not fail
	// Will not continue executing after throw
	if (currentNode->next == NULL) {
		THROW("Malloc for FilterNode struct returned NULL");
	}

	// Moves the currentNode pointer to the new node
	currentNode = currentNode->next;


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

			if (currentNode->next == NULL) {
				THROW("Malloc for FilterNode struct returned NULL");
			}

			currentNode = currentNode->next;

			currentNode->filter = newFilter;
			currentNode->next = tempNode;

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

	// root now points to a FilterNode struct
	root = malloc(sizeof(FilterNode));

	if (root == NULL) {
		THROW("Malloc for root FilterNode struct returned NULL");
	}

	// The chain is empty, so the root points to NULL
	root->next = NULL;

	return 0;
}
