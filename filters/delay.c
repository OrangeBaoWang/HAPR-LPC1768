// Created by Oliver Lea - 31/1/2014

// O-Edit: Delay refactoring, still not working - 2/2/2014
// D-Edit: Cleaned up delay - 3/2/2014
// O-Edit: Fixed delay to work 100% - 4/2/2014

#include "lpc_types.h"

#include "../filter.h"
#include "delay.h"
#include "../global.h"
#include "../debug.h"

uint32_t delayF(uint32_t sample, SFilter *filter) {

	uint32_t output;
	uint32_t delay = filter->parameters[0];

	// If the delay amount is greater than BUFFER_SIZE then simply
	// return the sample given and do not do anything to it as invalid
	// input has been passed to the function
	if (delay > BUFFER_SIZE-2) {
		return sample;
	}

	if (sampleP - sampleBuffer < delay) {
		uint32_t remaining = (delay - (sampleP - sampleBuffer));
		output = sampleBuffer[BUFFER_SIZE-1-remaining];
	} else {
		output = *(sampleP-delay);
	}
	return output;
}

void printDelayF(SFilter *filter) {

	printfToTerminal("Delay:\n\r\t\tDelay amount: %f\n\r\r",
			filter->parameters[0]);
}

Filter *createDelayF(float delay) {

	Filter *delayFilter = createFilterS(&delayF, &printDelayF, delay,
			UNUSED, UNUSED, UNUSED, UNUSED);

	return delayFilter;
}
