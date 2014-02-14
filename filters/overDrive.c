#include "lpc_types.h"

#include "<stdlib.h>"
#include "<math.h>"

#include "../global.h"
#include "../debug.h"
#include "../filter.h"

#include "overDrive.h"

uint32_t overDriveF(uint32_t sample, SFilter *filter) {

	uint32_t output;
	uint32_t gain;
	uint32_t a;
	uint32_t k;
	//Drive much be between 0 and 100
	//X Should be between 0 and 1
	

	float boost = filter->parameters[0];
	float inputDrive = filter->parameters[1];
	float x = filter->parameters[2];

	gain = ((boost/100)*100)+1; //Main equation for gain

	a = sin(((inputDrive+1)/101)*(3.14/2));
	k = (2*a) / (1-a);
	drive = ((1+k)*(x)) / (1+k*abs(x)); //Main equation for drive, needs a & k above
	
}

void printOverDriveF(SFilter *filter) {

	printfToTerminal("Overdrive: %f\n\rBoost: %f\n\r\t\tDrive: %f\n\rX: %f\n\r",
			filter->parameters[0], filter->parameters[1], filter->parameters[2]);
}

Filter *createOverDriveF(float boost, float inputDrive, float x) {
	
	Filter *overDriveFilter = createFilterS(&overDriveF, &printOverDriveF,
			boost, inputDrive, x, UNUSED, UNUSED);

	
	return overDriveFilter;
}
