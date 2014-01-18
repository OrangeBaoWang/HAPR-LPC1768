#ifndef __FILTER_H__
#define __FILTER_H__

// MUST BE INCLUDED ABOVE FILTERQUEUE.H"

// Structure containing all the information needed for a filter
// filterFunction points to the address of the destination filter
// function
typedef struct Filter {
	int (*filterFunction)(int, int);
	int parameters[2];
} Filter;

#endif
