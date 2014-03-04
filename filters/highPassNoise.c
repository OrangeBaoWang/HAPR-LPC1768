// Created by Dariush Bahri - 18/2/2014

// O-Edit: Changed file and function names - 26/1/2014

#include "lpc_types.h"

#include "../filter.h"
#include "highPassNoise.h"
#include "../global.h"
#include "../debug.h"

uint32_t highPassNoiseF(uint32_t sample, SFilter *filter) {
  
	float cutOff = filter->parameters[0];

	// If input sample is above cut off, return it
	if (sample > cutOff) {
	  return sample;
	}
	
	// Else return nothing
	else return AMPLITUDE_PIVOT;
}

void printHighPassNoiseF(SFilter *filter) {

	printfToTerminal("High-pass Noise Gate:\n\r\t\tCut off: %f\n\r\r",
			filter->parameters[0]);
}

Filter *createHighPassNoiseF(float cutOff) {

	Filter *highPassNoiseFilter = createFilterS(&highPassNoiseF, &printHighPassNoiseF, cutOff,
      UNUSED, UNUSED, UNUSED, UNUSED);

	return highPassNoiseFilter;
}
