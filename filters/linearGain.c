
#include "lpc_types.h"

#include "../filter.h"
#include "linearGain.h"
#include "../global.h"
#include "../debug.h"

// Simple linear gain filter that multiplies the input
// by the gain about zero volts which is 2.04V with bias
uint32_t linearGainF(uint32_t sample, float parameters[5]) {

	float gain = parameters[0];

	if (sample > 2048) {
		return (sample * gain);
	} else {
		return (sample - ((sample * gain) - sample));
	}
}

void printLinearGainF(float parameters[5]) {

	printfToTerminal("Linear Gain:\n\r\t\tGain Multiplier: %f\n\r\r", parameters[0]);
}

Filter *createLinearGainF(float gain) {

	Filter *linGainFilter = createFilterS(&linearGainF, gain,
			UNUSED, UNUSED, UNUSED, UNUSED);

	(linGainFilter->sfilter)->printFunction = &printLinearGainF;

	return linGainFilter;
}
