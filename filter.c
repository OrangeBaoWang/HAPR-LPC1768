#include "lpc_types.h"

#include "filter.h"
#include "filterChain.h"

uint32_t filterLookup[] =
			{&linearGainF,
			&parallelF};

char *filterNames[] =
			{"Linear gain",
			"Parallel filter"};

// Simple linear gain filter that multiplies the input
// by the gain
uint32_t linearGainF(uint32_t sample, uint32_t gainMuliplier) {

	return (sample * gainMultiplier);
}

// Mixes two filters outputs from the same sample with each other
// in order to produce an output that 
uint32_t parallelF(Filter *filter1, Filter *filter2,
							uint32_t sample, float mixRatio) {

	uint32_t outputFilter1 = (*(filter1->filterFunction))
								(sample, filter1->parameter);

	uint32_t outputFilter2 = (*(filter2->filterFunction))
								(sample, filter2->parameter);

	return ((outputFilter1 * mixRatio) + 
				(outputFilter2 * (1 - mixRatio)));

}

// Function to create each new filter struct so that it can be
// added to the filterNode struct upon enqueueing
Filter *newFilter(uint32_t filterAddr, uint32_t filterParam) {

	Filter *createdFilter;
	createdFilter = malloc(sizeof(Filter));

	createdFilter->filterFunction = filterAddr;
	createdFilter->parameter = filterParam;

	return createdFilter;
}

/*
FilterNode *newParFilter(Filter *filter1, Filter *filter2
							float mixRatio) {

	ParFilter *createdParFilter;
	createdParFilter = malloc(sizeof(ParFilter));

	createdParFilter->FilterOne = filter1;
	createdParFilter->FilterTwo = filter2;
	createdParFilter->mixingRatio = mixRatio;

	return createdParFilter;

}
*/
