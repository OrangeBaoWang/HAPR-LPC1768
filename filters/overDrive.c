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

	output = sample*boost; //boost should be between 1.8 and 4	
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
