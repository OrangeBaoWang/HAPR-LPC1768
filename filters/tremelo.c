#include "lpc_types.h"

#include <math.h>
#include <stdlib.h>

#include "../filter.h"
#include "tremelo.h"
#include "../global.h"
#include "../debug.h"

static float sineInput = 0;

uint32_t tremeloF(uint32_t sample, float parameters[5]) {

	uint32_t output;
	uint32_t sineOutput;

	float range = parameters[0];
	float frequency = parameters[1];

	float pivot = range / 2;

	// Calculates the sineIncrement from the given frequency
	float sineIncrement = frequency*6.3*23e-6;

	sineInput += sineIncrement;

	if (sineInput == 6.3) {
		sineInput = 0;
	}

	sineOutput = abs(sin(sineInput) * pivot);

	output = sineOutput * sample;

	return output;
}

void printTremeloF(float parameters[5]) {

	printfToTerminal("TREMELO:\n\r\t\tRange: %f\n\r\t\tFrequency: %f\n\r\r",
			parameters[0], parameters[1]);
}

Filter *createTremeloF(float range, float frequency) {

	Filter *tremeloFilter = createFilterS(&tremeloF, range, frequency,
			UNUSED, UNUSED, UNUSED);

	(tremeloFilter->sfilter)->printFunction = &printTremeloF;

	return tremeloFilter;
}
