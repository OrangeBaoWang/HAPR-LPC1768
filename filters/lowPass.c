#include "lpc_types.h"

#include "../filter.h"
#include "lowPass.h"
#include "../global.h"
#include "../debug.h"

uint32_t lowPassF(uint32_t sample, SFilter *filter) {
  float cutOff = filter->parameters[0];
  //if input sample is below cut off, return it
  if (sample < cutOff){
    return sample;
  }
  //else return nothing
  else return AMPLITUDE_PIVOT;
}

void printLowPassF(SFilter *filter) {

	printfToTerminal("Low Pass:\n\r\t\tCut off: %f\n\r\r",
			filter->parameters[0]);
}

Filter *createLowPassF(float cutOff) {

	Filter *lowPassFilter = createFilterS(&lowPassF, &printLowPassF, cutOff, UNUSED,
			UNUSED, UNUSED, UNUSED);

	return lowPassFilter;
}
