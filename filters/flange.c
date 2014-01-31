#include "lpc_types.h"

#include <math.h>
#include <stdlib.h>

#include "flange.h"
#include "global.h"
#include "debug.h"

// This global variable will be modified by the flangeF function
// in order to provide a correct input the next time the flange function
// is called
static float sineInput = 0;
static float mixingRatio = 0.6;

// Maximum range is 4095
uint32_t flangeF(uint32_t sample, float parameters[5]) {

	uint32_t output;
	uint32_t position;

	float sineOutput;

	float range = parameters[0];
	float pivot = range / 2;

	sineInput += 0.1;
	
	// If the sine wave has completed one full revolution, set it back
	// to zero
	if (sineInput == 6.6) {
		sineInput = 0;
	}

	sineOutput = sin(sineInput) * pivot;

	// Guard against range that is too large
	if (sampleP - abs(sineOutput) > sampleBuffer) {
		position = (BUFFER_SIZE / 2) + sineOutput;
	} else {
		THROW("INVALID FLANGE RANGE GIVEN");
	}

	output = (sample * mixingRatio) +
					((1-mixingRatio) * sampleBuffer[position]);

	return output;
}

void printFlangeF(float parameters[5]) {

	printfToTerminal("FLANGE:\n\r\t\tRange: %f\n\r\r", parameters[0]);
}
