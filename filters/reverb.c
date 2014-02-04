
#include "lpc_types.h"

#include "../filter.h"
#include "reverb.h"
#include "../global.h"
#include "../debug.h"

// A mixing ratio of 0.4 is optimal
uint32_t reverbF(uint32_t sample, float parameters[]) {

	float mixingRatio = parameters[0];
	uint32_t delay = parameters[1];

	uint32_t output;

	if (sampleP - sampleBuffer < delay) {
		uint32_t remaining = delay - (sampleP - sampleBuffer);
		output = (mixingRatio*sample) +
					((1-mixingRatio) * sampleBuffer[(BUFFER_SIZE)-1-remaining]);
	} else {
		output = (mixingRatio*sample) + ((1-mixingRatio) * (*(sampleP-delay)));
	}

	*sampleP = output;

	return output;
}

void printReverbF(float parameters[5]) {

	printfToTerminal("Reverb:\n\r\t\tMixing Ratio: %f\n\t\tDelay: %f\n\r\r",
			parameters[0], parameters[1]);
}

Filter *createReverbF(float mixingRatio, float delay) {

	Filter *reverbFilter = createFilterS(&reverbF, &printReverbF, mixingRatio, delay,
			UNUSED, UNUSED, UNUSED);

	return reverbFilter;
}
