// Created by Oliver Lea 17/01/2014

#ifndef __FILTERCHAIN_H__
#define __FILTERCHAIN_H__

typedef struct FilterNode {
	Filter *filter;
	struct FilterNode *next;
} FilterNode;

int enqueue(Filter *newFilter);

int dequeue(Filter *targetFilter);

int dequeueByIndex(float index);

int filterEq(SFilter *targetFilter, SFilter *currentFilter);

uint16_t applyFilters(uint16_t sample);

void printQueue(void);

int chain_init(void);

#endif
