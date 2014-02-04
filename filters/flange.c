#include "lpc_types.h"

#include <math.h>
#include <stdlib.h>

#include "../filter.h"
#include "flange.h"
#include "../global.h"
#include "../debug.h"

// This global variable will be modified by the flangeF function
// in order to provide a correct input the next time the flange function
// is called
static float sineInput = 0;

// Maximum range is 8000
// Range for strange effects is 8000
// Range for choral effect is 500
// Using a mixing ratio of 0.1 to 0.5 to give good results
// Standard flange effect for frequency = 0.1
// UFO effect for frequency = 10
// Formula 1 effect for frequency = 1
uint32_t flangeF(uint32_t sample, float parameters[5]) {

	uint32_t output;

	uint32_t sineOutput;

	float mixingRatio = parameters[0];
	float range = parameters[1];
	float sineIncrement = parameters[3];

	float pivot = range / 2;

	sineInput += sineIncrement;
	

	// If the sine wave has completed one full revolution, set it back
	// to zero
	if (sineInput == 6.3) {
		sineInput = 0;
	}

	sineOutput = abs(sin(sineInput) * pivot);

	// Guard against range that is too large
	if (sampleP - sampleBuffer > sineOutput) {
		output = (sample * mixingRatio) + ((1-mixingRatio) * (*(sampleP-sineOutput)));
	} else {
		uint32_t remaining = (sineOutput - (sampleP - sampleBuffer));
		output = ((sample * mixingRatio) +
				((1-mixingRatio) * (sampleBuffer[(BUFFER_SIZE)-1-remaining])));
	}


	return output;
}

void printFlangeF(float parameters[5]) {

	printfToTerminal("FLANGE:\n\r\t\tMixingRatio: %f\n\r\t\tRange: %f\n\r\t\tFrequency: %f\n\r\r",
			parameters[0], parameters[1], parameters[2]);
}

Filter *createFlangeF(float mixingRatio, float range, float frequency) {
	
	float sineIncrement = frequency*6.3*23e-6;

	Filter *flangeFilter = createFilterS(&flangeF, &printFlangeF,
			mixingRatio, range, frequency, sineIncrement,
			UNUSED);

	return flangeFilter;
}
