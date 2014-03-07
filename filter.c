// Created by Oliver Lea - 20/01/2014

// O-Edit: Added Filter creation functions - 21/01/2014
// O-Edit: Changed Filter creation functions to take floats as parameters instead
//	of uint32_t's - 22/01/2014
// O-Edit: Moved all filters out of filter.c and into there own folder in
//	individual files - 31/01/2014
// O-Edit: Modified createFilterP to take a Filter structs as parameters instead
//	of SFilter structs - 02/02/2014
// O-Edit: Added passing the printing function pointer for each filter to the
//	createFilterS function in order to facilitate printing the filters - 04/02/2014
// O-Edit: Added free() ing of memory that was no longer needed in the createFilterP
//	function - 20/02/2014
// O-Edit: Added assertions that no malloc() returns NULL in all functions
//	in filter.c - 28/02/2014

#include <stdlib.h>
#include <math.h>

#include "lpc_types.h"

#include "global.h"
#include "debug.h"
#include "filter.h"
#include "filterChain.h"

// Mixes two filters outputs from the same sample with each other
// in order to produce an output that 
uint32_t mixParallel(PFilter *pfilter, uint32_t sample) {

	uint32_t outputFilter1 = (*((pfilter->filterOne)->filterFunction))
					(sample, pfilter->filterOne);

	uint32_t outputFilter2 = (*((pfilter->filterTwo)->filterFunction))
					(sample, pfilter->filterTwo);

	return (uint32_t)(((outputFilter1 * (pfilter->mixRatio)) +
				(outputFilter2 * (1.0 - (pfilter->mixRatio)))));

}

// Function to create each new filter struct so that it can be
// added to the filterNode struct upon enqueueing
Filter *createFilterS(uint32_t (*filterAddr)(uint32_t, SFilter *), void (*printAddr)(SFilter *),
		float param0, float param1, float param2, float param3, float param4) {

	SFilter *createdSfilter;
	createdSfilter = malloc(sizeof(SFilter));

	if (createdSfilter == NULL) {
		THROW("Malloc for SFilter struct returned NULL");
	}

	Filter *createdFilter;
	createdFilter = malloc(sizeof(Filter));

	if (createdFilter == NULL) {
		THROW("Malloc for Filter struct returned NULL");
	}

	createdFilter->sfilter = createdSfilter;
	createdFilter->pfilter = NULL;

	createdSfilter->filterFunction = filterAddr;
	createdSfilter->printFunction = printAddr;
	createdSfilter->parameters[0] = param0;
	createdSfilter->parameters[1] = param1;
	createdSfilter->parameters[2] = param2;
	createdSfilter->parameters[3] = param3;
	createdSfilter->parameters[4] = param4;

	return createdFilter;
}

// Function to create a new filter struct that will be added to the
// filterNode struct upon enqueueing
Filter *createFilterP(Filter *filter1, Filter *filter2,
					float mixingRatio) {

	PFilter *createdPfilter;
	createdPfilter = malloc(sizeof(PFilter));

	if (createdPfilter == NULL) {
		THROW("Malloc for PFilter struct returned NULL");
	}

	Filter *createdFilter;
	createdFilter = malloc(sizeof(Filter));

	if (createdFilter == NULL) {
		THROW("Malloc for Filter struct returned NULL");
	}

	createdFilter->sfilter = NULL;
	createdFilter->pfilter = createdPfilter;

	createdPfilter->filterOne = filter1->sfilter;
	createdPfilter->filterTwo = filter2->sfilter;
	createdPfilter->mixRatio = mixingRatio;

	// Free the now unused filter structs passed into the function
	// as only the sfilters they contain are used
	free(filter1);
	free(filter2);

	return createdFilter;
}
