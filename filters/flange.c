#include "lpc_types.h"

#include <math.h>
#include <stdlib.h>

#include "../filter.h"
#include "flange.h"
#include "../global.h"
#include "../debug.h"

#define SINE_INPUT filter->scratch[0]
#define SINE_INCREMENT filter->scratch[1]

// Maximum range is 8000
// Range for strange effects is 8000
// Range for choral effect is 500
// Using a mixing ratio of 0.1 to 0.5 to give good results
// Standard flange effect for frequency = 0.1
// UFO effect for frequency = 10
// Formula 1 effect for frequency = 1
uint32_t flangeF(uint32_t sample, SFilter *filter) {

	uint32_t output;

	uint32_t sineOutput;

	float mixingRatio = filter->parameters[0];
	float range = filter->parameters[1];

	float pivot = range / 2;

	SINE_INPUT += SINE_INCREMENT;

	// If the sine wave has completed one full revolution, set it back
	// to zero
	if (SINE_INPUT == 6.3) {
		SINE_INPUT = 0;
	}

	sineOutput = abs(sin(SINE_INPUT) * pivot);

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

void printFlangeF(SFilter *filter) {

	printfToTerminal("FLANGE:\n\r\t\tMixingRatio: %f\n\r\t\tRange: %f\n\r\t\tFrequency: %f\n\r\r",
			filter->parameters[0], filter->parameters[1], filter->parameters[2]);
}

Filter *createFlangeF(float mixingRatio, float range, float frequency) {
	
	float sineIncrement = frequency*6.3*23e-6;

	Filter *flangeFilter = createFilterS(&flangeF, &printFlangeF,
			mixingRatio, range, frequency, UNUSED, UNUSED);

	// The initial value to be fed into the sin() function
	(flangeFilter->sfilter)->scratch[0] = 0;
	(flangeFilter->sfilter)->scratch[1] = sineIncrement;

	return flangeFilter;
}
