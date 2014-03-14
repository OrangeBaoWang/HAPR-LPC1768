// Created by Oliver Lea 18/01/2014

// O-Edit: Defined SFilter, PFilter and Filter structs, and added Filter creation 
//	functions - 21/01/2014

// O-Edit: Changed Filter creation functions to take floats as parameters instead
//	of uint32_t's - 22/01/2014

// O-Edit: Moved all filters out of filter.c and into there own folder in
//	individual files - 31/01/2014

// O-Edit: Modified createFilterP to take a Filter structs as parameters instead
//	of SFilter structs - 02/02/2014

// O-Edit: Added passing the printing function pointer for each filter to the
//	createFilterS function in order to facilitate printing the filters - 04/02/2014

#ifndef __FILTER_H__
#define __FILTER_H__

// MUST BE INCLUDED ABOVE FILTERCHAIN.H"

// Structure containing all the information needed for a filter
// filterFunction points to the address of the destination filter function
// printFunction points to the address of the destination filter printing function

typedef struct SFilter {
	uint32_t (*filterFunction)(uint32_t, struct SFilter *);
	void (*printFunction)(struct SFilter *);
	float parameters[5];
	float scratch[5];
} SFilter;

typedef struct PFilter {
	SFilter *filterOne;
	SFilter *filterTwo;
	float mixRatio;
} PFilter;

typedef struct Filter {
	PFilter *pfilter;
	SFilter *sfilter;
} Filter;

uint32_t mixParallel(PFilter *pfilter, uint32_t sample);

Filter *createFilterS(uint32_t (*filterAddr)(uint32_t, SFilter *), void (*printAddr)(SFilter *),
		float param0, float param1, float param2, float param3, float param4);

Filter *createFilterP(Filter *filter1, Filter *filter2, float mixingRatio);

#endif
