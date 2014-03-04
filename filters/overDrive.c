// Created by James Lee - 13/2/2014

// J-Edit: Changed first equations for overdrive - 18/2/2014
// J-Edit: Changed second set of equations - 20/2/2014
// J-Edit: Finalised third set of equations - 25/2/2014

#include "lpc_types.h"

#include <stdlib.h>
#include <math.h>

#include "../filter.h"
#include "overDrive.h"
#include "../global.h"
#include "../debug.h"

uint32_t overdriveF(uint32_t sample, SFilter *filter) {

	uint32_t output;
	float boost = filter->parameters[0];

	output = sample*boost; //boost should be between 1.5 and 2	
	return output;
}

void printOverdriveF(SFilter *filter) {

	printfToTerminal("Overdrive: \n\r\t\tBoost: %f\n\r\t\t",
			filter->parameters[0]);
}

Filter *createOverdriveF(float boost) {
	
	Filter *overdriveFilter = createFilterS(&overdriveF, &printOverdriveF,
			boost, UNUSED, UNUSED, UNUSED, UNUSED);

	printfToTerminal("Boost: %f\n\r", boost);

	(overdriveFilter->sfilter)->parameters[0] = boost;

	return overdriveFilter;
}

