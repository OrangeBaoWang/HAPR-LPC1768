// Created by Oliver Lea 17/01/2014

#include "lpc_types.h"

#include "stdlib.h"

#include "debug.h"
#include "filter.h"
#include "filterChain.h"
#include "envFollower.h"

FilterNode *currentNode;
FilterNode *root;

// The 32-bit container the sample is put into while filtering
// takes place
uint32_t dSample;

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

	if (targetFilter->pfilter == NULL) {

		while (currentNode->next != 0) {

			if (((currentNode->next)->filter)->pfilter == NULL) {
				if (filterEq(targetFilter->sfilter, ((currentNode->next)->filter)->sfilter) == 1) {

					currentNode->next = (currentNode->next)->next;
					return 0;
				}
			}
			currentNode = currentNode->next;
		}
	} else {
		// If the filter is a parallel filter
		while (currentNode->next != 0) {
			if (((currentNode->next)->filter)->pfilter != NULL) {
				if (filterEq((targetFilter->pfilter)->filterOne,
						(((currentNode->next)->filter)->pfilter)->filterOne) == 1) {

					if (filterEq((targetFilter->pfilter)->filterTwo,
							(((currentNode->next)->filter)->pfilter)->filterTwo) == 1) {

						currentNode->next = (currentNode->next)->next;
						return 0;
					}
				}
			}
			currentNode = currentNode->next;
		}
	}	
	return -1;
}

// Will return 1 if two filters are equivalent, 0 otherwise
int filterEq(SFilter *targetFilter, SFilter *currentFilter) {

	if ((targetFilter->filterFunction) == (currentFilter->filterFunction)) {
		if ((targetFilter->parameter) == (currentFilter->parameter)) {
			return 1;
		}
	}
	return 0;
}


// Will not increment the currentNode if already at the end
// of the list
void incrementCurrentNode(void) {

	if (currentNode->next != 0) {
		currentNode = currentNode->next;
	}
}

// Will apply all filters present in the filter chain
// If no filters in chain, simply return the given value
uint16_t applyFilters(uint16_t sample) {

	if (root->next == 0) {
		return sample;
	}

	dSample = sample;

	currentNode = root;

	// While not at the end of the list, apply each filter to
	// the value in dSample
	while(currentNode->next != 0) {

		currentNode = currentNode->next;

		if ((currentNode->filter)->sfilter == 0) {
			dSample = mixParallel((currentNode->filter)->pfilter,
						dSample);
		} else {
			dSample =
				(*(((currentNode->filter)->sfilter)->filterFunction))
						(dSample, (((currentNode->filter)->sfilter)
						->parameter));
		}
	}

	// If the value from the filters is greater than 2^12 (the
	// highest value of the ADC, sample is clipped to 2^12
	if (dSample > 4096) {
		sample = 4096;
	} else {sample = dSample;}


	return (sample);
}

int chain_init(void) {

	// root now points to a node struct
	root = malloc(sizeof(FilterNode));

	if (root == NULL) {
		WARN(0, "Malloc failed for filterQueue");
		return -1;
	}

	// The node pointer points to null
	root->next = 0;

	currentNode = root;

	// Initialises the variables necessary for envelope follower filters
	envFollowerInit();

	return 0;
}
