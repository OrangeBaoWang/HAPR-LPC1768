
#include "lpc_types.h"

#include "reverb.h"
#include "../global.h"
#include "../debug.h"

// A mixing ratio of 0.4 is optimal
uint32_t reverbF(uint32_t sample, float parameters[]) {

	float mixingRatio = parameters[0];

	uint32_t output;

	if (sampleP - sampleBuffer < 8000) {
		uint32_t remaining = 8000 - (sampleP - sampleBuffer);
		output = (mixingRatio*sample) +
					((1-mixingRatio) * sampleBuffer[(BUFFER_SIZE)-1-remaining]);
	} else {
		output = (mixingRatio*sample) + ((1-mixingRatio) * (*(sampleP-8000)));
	}

	*sampleP = output;

	return output;
}

void printReverbF(float parameters[5]) {

	printfToTerminal("Reverb:\n\r\t\tMixing Ratio: %f\n\r\r", parameters[0]);
}
