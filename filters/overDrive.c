#include "lpc_types.h"
#include "../filter.h"
#include "overDrive.h"
#include "../global.h"
#include "../debug.h"
#include "<stdlib.h>"
#include "<math.h>"

uint32_t overDriveF(uint32_t sample, SFilter *filter) {

	uint32_t output;
	uint32_t boost;
	uint32_t a;
	uint32_t k;
	uint32_t inputDrive; //Drive much be between 0 and 100
	uint32_t x; //Should be between 0 and 1
	

	float gain = filter->parameters[0];
	float drive = filter->parameters[1];

	gain = ((boost/100)*100)+1; //Main equation for gain

	a = sin(((inputDrive+1)/101)*(3.14/2));
	k = (2*a) / (1-a);
	drive = ((1+k)*(x)) / (1+k*abs(x)); //Main equation for drive, needs a & k above
	




void printOverDriveF(SFilter *filter) {

	printfToTerminal("Overdrive:\n\r\t\tGain: %f\n\r\t\tDrive: %f\n\r\t\t",
			filter->parameters[0], filter->parameters[1]);
}

Filter *createOverDriveF(float gain, float drive) {
	
	Filter *overDriveFilter = createFilterS(&overDriveF, &printOverDriveF,
			gain, drive, UNUSED, UNUSED, UNUSED);

	
	return overDriveFilter;
}
