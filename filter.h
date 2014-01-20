// Created by Oliver Lea 18/01/2014

#ifndef __FILTER_H__
#define __FILTER_H__

// MUST BE INCLUDED ABOVE FILTERCHAIN.H"

// Structure containing all the information needed for a filter
// filterFunction points to the address of the destination filter
// function
typedef struct Filter {
	uint32_t (*filterFunction)(uint32_t, uint32_t);
	uint32_t parameter;
} Filter;

/*
typedef struct ParFilter {
	Filter *filterOne;
	Filter *filterTwo;
	float mixingRatio;
} ParFilter;
*/

uint32_t linearGainF(uint32_t sample, uint32_t gainMultiplier);

uint32_t parallelF(Filter *filter1, Filter *filter2,
						uint32_t sample, float mixRatio);

Filter *newFilter(uint32_t filterAddr, uint32_t filterParam);

#endif
