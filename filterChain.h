// Created by Oliver Lea 17/01/2014

// O-Edit: Defined the FilterNode struct that is the top-level componenet of the filter
//	chain - 18/01/2014

// O-Edit: Added apply filters function that takes the current chain of filters
//	and the sample to be filtered, and returns the post-processed sample - 19/01/2014

// O-Edit: Added dequeuing parallel filters to the dequeue() function. Also added a
//	filterEq() function that tests if two SFilters are equivalent - 23/01/2014

// O-Edit: Began work on a printQueue() function that would allow the filter chain
//	to be printed from the UI. Work in progress - 29/01/2014

// O-Edit: Modified filterEq(), applyFilters() and the (unfinished) printQueue()
//	functions to support a change in how filter parameters are passed (Used to be
//	passed as two floats, but are now passed as an array of five floats) - 31/01/2014

// O-Edit: dequeueByIndex() function added that removes a filter from the filter
//	chain according to its index - 12/02/2014

// O-Edit: enqueueByIndex() function added that adds a filter to the filter chain
//	in a specified position given by the index passed to the function - 17/02/2014

// O-Edit: Added freeNode() function (frees previously malloc'd space now unused by
//	old filters) and dequeueAll() function (empties the filter chain) - 18/02/2014

#ifndef __FILTERCHAIN_H__
#define __FILTERCHAIN_H__

typedef struct FilterNode {
	Filter *filter;
	struct FilterNode *next;
} FilterNode;

void freeNode(FilterNode *nodeToFree);

void enqueue(Filter *newFilter);

int enqueueByIndex(Filter *newFilter, float index);

int dequeue(Filter *targetFilter);

int dequeueByIndex(float index);

int dequeueAll(void);

int filterEq(SFilter *targetFilter, SFilter *currentFilter);

uint16_t applyFilters(uint16_t sample);

void printQueue(void);

int chain_init(void);

#endif
