// Created by Oliver Lea - 04/02/2014

// O-Edit: Fixed equality bug - 12/02/2014

#include "lpc_types.h"

#include <math.h>
#include <stdlib.h>

#include "../filter.h"
#include "tremelo.h"
#include "../global.h"
#include "../debug.h"

#define SINE_INCREMENT filter->scratch[0]
#define SINE_INPUT filter->scratch[1]

// Range must be between 0 and 1 to avoid saturation
// Range value of 0.7 works well
// Frequency value of 2 makes things very noticeable
uint32_t tremeloF(uint32_t sample, SFilter *filter) {

	uint32_t output;
	float sineOutput;

	float range = filter->parameters[0];

	SINE_INPUT += SINE_INCREMENT;

	if (SINE_INPUT >= 6.3) {
		SINE_INPUT = 0;
	}

	sineOutput = sin(SINE_INPUT);

	// If the value of the sine would be negative, decrease value of the sample
	// Else, increase the value of the sample
	if (SINE_INPUT > 3.15) {
		output = sample - (sineOutput * (range*sample));
	} else {
		output = sample + (sineOutput * (range*sample));
	}

	return (output);
}

void printTremeloF(SFilter *filter) {

	printfToTerminal("Tremelo:\n\r\t\tRange: %f\n\r\t\tFrequency: %f\n\r\r",
			filter->parameters[0], filter->parameters[1]);
}

Filter *createTremeloF(float range, float frequency) {

	Filter *tremeloFilter = createFilterS(&tremeloF, &printTremeloF, range, frequency,
			UNUSED, UNUSED, UNUSED);

	(tremeloFilter->sfilter)->scratch[0] = frequency*6.3*23e-6;
	(tremeloFilter->sfilter)->scratch[1] = 0;

	return tremeloFilter;
}
