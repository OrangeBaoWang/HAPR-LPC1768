#include "lpc_types.h"

#include <math.h>
#include <stdlib.h>

#include "../filter.h"
#include "tremelo.h"
#include "../global.h"
#include "../debug.h"

static float sineInput = 0;

// Range vmust be between 0 and 1 to avoid saturation
// Range value of 0.7 works well
// Frequency value of 2 makes things very noticeable
uint32_t tremeloF(uint32_t sample, float parameters[5]) {

	uint32_t output;
	float sineOutput;

	float range = parameters[0];

	// Calculates the sineIncrement from the given frequency
	float sineIncrement = parameters[2];

	sineInput += sineIncrement;

	if (sineInput > 6.3) {
		sineInput = 0;
	}

	sineOutput = sin(sineInput);

	// If the value of the sine would be negative, decrease value of the sample
	// Else, increase the value of the sample
	if (sineInput > 3.15) {
		output = sample - (sineOutput * (range*sample));
	} else {
		output = sample + (sineOutput * (range*sample));
	}


	return (output);
}

void printTremeloF(float parameters[5]) {

	printfToTerminal("TREMELO:\n\r\t\tRange: %f\n\r\t\tFrequency: %f\n\r\r",
			parameters[0], parameters[1]);
}

Filter *createTremeloF(float range, float frequency) {

	float sineIncrement = frequency*6.3*23e-6;

	Filter *tremeloFilter = createFilterS(&tremeloF, &printTremeloF, range, frequency, sineIncrement,
			UNUSED, UNUSED);

	return tremeloFilter;
}
