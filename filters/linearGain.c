// Created by Oliver Lea - 31/1/2014

// O-Edit: Added linear gain init method - 2/2/2014
// J-Edit: Tested and found bug - 5/2/2014
// O-Edit: Fixed bug that caused huge distortion - 20/2/2014



#include "lpc_types.h"

#include "../filter.h"
#include "linearGain.h"
#include "../global.h"
#include "../debug.h"

// Simple linear gain filter that multiplies the input
// by the gain about zero volts which is 2.04V with bias
uint32_t linearGainF(uint32_t sample, SFilter *filter) {

	float gain = filter->parameters[0];

	if (sample > AMPLITUDE_PIVOT) {
		return (sample * gain);
	} else {
		return (sample * (1 - (1-gain)));
	}
}

void printLinearGainF(SFilter *filter) {

	printfToTerminal("Linear Gain:\n\r\t\tGain Multiplier: %f\n\r\r",
			filter->parameters[0]);
}

Filter *createLinearGainF(float gain) {

	Filter *linGainFilter = createFilterS(&linearGainF, &printLinearGainF, gain,
			UNUSED, UNUSED, UNUSED, UNUSED);

	return linGainFilter;
}
