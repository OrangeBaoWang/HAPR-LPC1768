// Created by Oliver Lea 17/01/2014

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
