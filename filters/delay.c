
#include "lpc_types.h"

#include "delay.h"
#include "../global.h"
#include "../debug.h"

uint32_t delayF(uint32_t sample, float parameters[5]) {

	uint32_t output;

	if (sampleP - sampleBuffer < 8000) {
		uint32_t remaining = (8000 - (sampleP - sampleBuffer));
		output = sampleBuffer[BUFFER_SIZE-1-remaining];
	} else {
		output = *(sampleP-8000);
	}
	return output;
}

void printDelayF(float parameters[5]) {

	printfToTerminal("Delay:\n\r\t\tParameters: NULL\n\r\r");
}
