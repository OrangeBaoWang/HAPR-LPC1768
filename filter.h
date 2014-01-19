// Created by Oliver Lea 18/01/2014

#ifndef __FILTER_H__
#define __FILTER_H__

// MUST BE INCLUDED ABOVE FILTERCHAIN.H"

// Structure containing all the information needed for a filter
// filterFunction points to the address of the destination filter
// function
typedef struct Filter {
	uint64_t (*filterFunction)(uint64_t, int);
	int parameter;
} Filter;

#endif
