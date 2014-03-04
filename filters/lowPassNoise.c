#include "lpc_types.h"

#include "../filter.h"
#include "lowPassNoise.h"
#include "../global.h"
#include "../debug.h"

uint32_t lowPassNoiseF(uint32_t sample, SFilter *filter) {
  float cutOff = filter->parameters[0];
  //if input sample is below cut off, return it
  if (sample < cutOff){
    return sample;
  }
  //else return nothing
  else return AMPLITUDE_PIVOT;
}

void printLowPassNoiseF(SFilter *filter) {

	printfToTerminal("Low-pass Noise Gate:\n\r\t\tCut off: %f\n\r\r",
			filter->parameters[0]);
}

Filter *createLowPassNoiseF(float cutOff) {

	Filter *lowPassNoiseFilter = createFilterS(&lowPassNoiseF, &printLowPassNoiseF, cutOff, UNUSED,
			UNUSED, UNUSED, UNUSED);

	return lowPassNoiseFilter;
}
