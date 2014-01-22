#include "stdlib.h"

#include "lpc_types.h"

#include "global.h"
#include "debug.h"
#include "filter.h"
#include "filterChain.h"


// Simple linear gain filter that multiplies the input
// by the gain
uint32_t linearGainF(uint32_t sample, float gainMultiplier) {

	return (sample * gainMultiplier);
}

uint32_t delayF(uint32_t sample, float nullVar) {

	uint32_t output;

	// Multiply the input value by reverbParam
	// Multiply the reverbParam by the sample position in the
	// queue - X where x is a constant. Eg, 2000 places.
	// Add the two values together


	if (sampleP - sampleBuffer < 8000) {
		uint32_t remaining = (8000 - (sampleP - sampleBuffer));
		output = sampleBuffer[BUFFER_SIZE-1-remaining];
	} else {
		output = *(sampleP-8000);
	}
	return output;
}

uint32_t echoF(uint32_t sample, float nullVar) {

	uint32_t output;

	if (sampleP - sampleBuffer < 8000) {
		uint32_t remaining = 8000 - (sampleP - sampleBuffer);
		output = (0.8*sample) + 
					(0.2 * sampleBuffer[(BUFFER_SIZE)-1-remaining]);
	} else {
		output = (0.8*sample) + (0.2 * (*(sampleP-8000)));
	}

	return output;
}

uint32_t reverbF(uint32_t sample, float nullVar) {

	uint32_t output;

	if (sampleP - sampleBuffer < 8000) {
		uint32_t remaining = 8000 - (sampleP - sampleBuffer);
		output = (0.4*sample) +
					(0.6 * sampleBuffer[(BUFFER_SIZE)-1-remaining]);
	} else {
		output = (0.4*sample) + (0.6 * (*(sampleP-8000)));
	}

	//printfToTerminal("Input = %d, output = %d\n\r", sample, output);

	*sampleP = output;

	return output;
}

// Mixes two filters outputs from the same sample with each other
// in order to produce an output that 
uint32_t mixParallel(PFilter *pfilter, uint32_t sample) {

	uint32_t outputFilter1 = (*((pfilter->filterOne)->filterFunction))
							(sample, (pfilter->filterOne)->parameter);

	uint32_t outputFilter2 = (*((pfilter->filterTwo)->filterFunction))
							(sample, (pfilter->filterTwo)->parameter);

	return (uint32_t)(((outputFilter1 * (pfilter->mixRatio)) +
				(outputFilter2 * (1.0 - (pfilter->mixRatio)))));

}

// Function that will create a new SFilter struct for adding to
// PFilter structs for parallel filters
SFilter *newSfilter(uint32_t (*filterAddr)(uint32_t, float),
					float filterParam) {

	SFilter *createdSfilter;
	createdSfilter = malloc(sizeof(SFilter));

	createdSfilter->filterFunction = filterAddr;
	createdSfilter->parameter = filterParam;

	return createdSfilter;
}

// Function to create each new filter struct so that it can be
// added to the filterNode struct upon enqueueing
Filter *createFilterS(uint32_t (*filterAddr)(uint32_t, float),
					float filterParam) {

	Filter *createdFilter;
	createdFilter = malloc(sizeof(Filter));

	SFilter *createdSfilter;
	createdSfilter = malloc(sizeof(SFilter));

	createdFilter->sfilter = createdSfilter;
	createdFilter->pfilter = 0;
	createdFilter->parallel = 0;

	createdSfilter->filterFunction = filterAddr;
	createdSfilter->parameter = filterParam;

	return createdFilter;
}

// Function to create a new filter struct that will be added to the
// filterNode struct upon enqueueing
Filter *createFilterP(SFilter *sfilter1, SFilter *sfilter2,
					float mixingRatio) {

	Filter *createdFilter;
	createdFilter = malloc(sizeof(Filter));

	PFilter *createdPfilter;
	createdPfilter = malloc(sizeof(PFilter));

	createdFilter->sfilter = 0;
	createdFilter->pfilter = createdPfilter;
	createdFilter->parallel = 1;

	createdPfilter->filterOne = sfilter1;
	createdPfilter->filterTwo = sfilter2;
	createdPfilter->mixRatio = mixingRatio;

	return createdFilter;
}
