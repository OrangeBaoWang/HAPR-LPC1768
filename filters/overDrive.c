#include "lpc_types.h"

#include <stdlib.h>
#include <math.h>

#include "../filter.h"
#include "overDrive.h"
#include "../global.h"
#include "../debug.h"

#define DRIVE filter->scratch[0]
#define GAIN filter->scratch[1]

uint32_t overdriveF(uint32_t sample, SFilter *filter) {

	uint32_t output;

	//Drive much be between 0 and 100
	//X Should be between 0 and 1
	
	output = (sample*GAIN)*DRIVE;
	
	return output;
}

void printOverdriveF(SFilter *filter) {

	printfToTerminal("Overdrive: %f\n\r\t\tBoost: %f\n\r\t\tDrive: %f\n\r\t\tX: %f\n\r",
			filter->parameters[0], filter->parameters[1], filter->parameters[2]);
}

Filter *createOverdriveF(float boost, float inputDrive, float x) {
	
	Filter *overdriveFilter = createFilterS(&overdriveF, &printOverdriveF,
			boost, inputDrive, x, UNUSED, UNUSED);

	uint32_t a = sin(((inputDrive+1)/101)*(3.14/2));
	uint32_t k = (2*a) / (1-a);
	uint32_t drive = ((1+k)*(x)) / (1+k*abs(x)); //Main equation for drive, needs a & k above
	uint32_t gain = ((boost/100)*100)+1; //Main equation for gain

	(overdriveFilter->sfilter)->scratch[0] = drive;
	(overdriveFilter->sfilter)->scratch[1] = gain;

	return overdriveFilter;
}
