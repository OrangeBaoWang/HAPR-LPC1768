
#include "lpc_types.h"

#include "../filter.h"
#include "echo.h"
#include "../global.h"
#include "../debug.h"

// A mixing ratio of 0.8 is optimal
// with a delay of 8000
uint32_t echoF(uint32_t sample, float parameters[5]) {

	uint32_t output;

	float mixingRatio = parameters[0];
	uint32_t delay = parameters[1];

	// Guard against large user input for delay
	if (delay > BUFFER_SIZE-2) {
		return sample;
	}

	if (sampleP - sampleBuffer < delay) {
		uint32_t remaining = delay - (sampleP - sampleBuffer);
		output = (mixingRatio*sample) + 
					((1-mixingRatio) * sampleBuffer[(BUFFER_SIZE)-1-remaining]);
	} else {
		output = (mixingRatio*sample) + ((1-mixingRatio) * (*(sampleP-delay)));
	}

	return output;
}

void printEchoF(float parameters[5]) {

	printfToTerminal("Delay:\n\r\t\tMixing Ratio: %f\n\r\t\tDelay: %f\n\r\r",
			parameters[0], parameters[1]);
}

Filter *createEchoF(float mixingRatio, float delay) {

	Filter *echoFilter = createFilterS(&echoF, mixingRatio, delay,
			UNUSED, UNUSED, UNUSED);

	(echoFilter->sfilter)->printFunction = &printEchoF;

	return echoFilter;
}
