
#include "lpc_types.h"

#include "../filter.h"
#include "reverb.h"
#include "../global.h"
#include "../debug.h"

// A mixing ratio of 0.4 is optimal
uint32_t reverbF(uint32_t sample, SFilter *filter) {

	float mixingRatio = filter->parameters[0];
	uint32_t delay = filter->parameters[1];

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

void printReverbF(SFilter *filter) {

	printfToTerminal("REVERB:\n\r\t\tMixing Ratio: %f\n\r\t\tDelay: %f\n\r\r",
			filter->parameters[0], filter->parameters[1]);
}

Filter *createReverbF(float mixingRatio, float delay) {

	Filter *reverbFilter = createFilterS(&reverbF, &printReverbF, mixingRatio, delay,
			UNUSED, UNUSED, UNUSED);

	return reverbFilter;
}
