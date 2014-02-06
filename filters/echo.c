
#include "lpc_types.h"

#include "../filter.h"
#include "echo.h"
#include "../global.h"
#include "../debug.h"

// A mixing ratio of 0.8 is optimal
// with a delay of 8000
uint32_t echoF(uint32_t sample, SFilter *filter) {

	uint32_t output;

	float mixingRatio = filter->parameters[0];
	uint32_t delay = filter->parameters[1];

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

void printEchoF(SFilter *filter) {

	printfToTerminal("Echo:\n\r\t\tMixing Ratio: %f\n\r\t\tEcho: %f\n\r\r",
			filter->parameters[0], filter->parameters[1]);
}

Filter *createEchoF(float mixingRatio, float delay) {

	Filter *echoFilter = createFilterS(&echoF, &printEchoF, mixingRatio, delay,
			UNUSED, UNUSED, UNUSED);

	return echoFilter;
}
