#include "lpc_types.h"

#include "../filter.h"
#include "highPass.h"
#include "../global.h"
#include "../debug.h"

uint32_t highPassF(uint32_t sample, SFilter *filter) {
  
  float cutOff = filter->parameters[0];

  //if input sample is above cut off, return it
  if (sample > cutOff){
    return sample;
  }
  //else return nothing
  else return AMPLITUDE_PIVOT;
}

void printHighPassF(SFilter *filter) {

	printfToTerminal("High Pass:\n\r\t\tCut off: %f\n\r\r",
			filter->parameters[0]);
}

Filter *createHighPassF(float cutOff) {

	Filter *highPassFilter = createFilterS(&highPassF, &printHighPassF, cutOff, UNUSED,
		  UNUSED, UNUSED, UNUSED);

	return highPassFilter;
}
