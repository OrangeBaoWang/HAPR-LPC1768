
#include "lpc_types.h"

#include "../filter.h"
#include "delay.h"
#include "../global.h"
#include "../debug.h"

uint32_t delayF(uint32_t sample, float parameters[5]) {

	uint32_t output;
	uint32_t delay = parameters[0];

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

void printDelayF(float parameters[5]) {

	printfToTerminal("Delay:\n\r\t\tDelay amount: %f\n\r\r", parameters[0]);
}

Filter *createDelayF(float delay) {

	Filter *delayFilter = createFilterS(&delayF, &printDelayF, delay,
			UNUSED, UNUSED, UNUSED, UNUSED);

	return delayFilter;
}
