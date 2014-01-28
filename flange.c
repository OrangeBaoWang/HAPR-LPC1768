#include "lpc_types.h"

#include <math.h>
#include <stdlib.h>

#include "global.h"
#include "flange.h"
#include "debug.h"

// This global variable will be modified by the flangeF function
// in order to provide a correct input the next time the flange function
// is called
float sineInput = 0;
float mixingRatio = 0.6;


// Maximum range is 4095
uint32_t flangeF(uint32_t sample, float range) {

	uint32_t output;
	uint32_t position;
	float sineOutput;
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
