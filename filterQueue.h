#ifndef __FILTERQUEUE_H__
#define __FILTERQUEUE_H__

typedef struct FilterNode {
	Filter *filter;
	struct FilterNode *next;
} FilterNode;

int enqueue(Filter *newFilter);

int dequeue(Filter *targetFilter);

void incrementCurrentNode(void);

int queueInit(void);

#endif
