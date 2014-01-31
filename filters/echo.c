
#include "lpc_types.h"

#include "echo.h"
#include "../global.h"
#include "../debug.h"

// A mixing ratio of 0.8 is optimal
uint32_t echoF(uint32_t sample, float parameters[5]) {

	float mixingRatio = parameters[0];

	uint32_t output;

	if (sampleP - sampleBuffer < 8000) {
		uint32_t remaining = 8000 - (sampleP - sampleBuffer);
		output = (mixingRatio*sample) + 
					((1-mixingRatio) * sampleBuffer[(BUFFER_SIZE)-1-remaining]);
	} else {
		output = (mixingRatio*sample) + ((1-mixingRatio) * (*(sampleP-8000)));
	}

	return output;
}

void printEchoF(float parameters[5]) {

	printfToTerminal("Delay:\n\r\t\tMixing Ratio: %f\n\r\r", parameters[0]);
}
