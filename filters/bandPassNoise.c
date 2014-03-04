// Created by Dariush Bahri - 27/2/2014

// O-Edit: Changed names of functions to match main - 2/3/2014

#include "lpc_types.h"

#include "../filter.h"
#include "bandPassNoise.h"
#include "../global.h"
#include "../debug.h"

uint32_t bandPassNoiseF(uint32_t sample, SFilter *filter) {
  
  float cutOffLow = filter->parameters[0];
  float cutOffHigh = filter->parameters[1];
  
  //if sample is above cutOffLow and below cutOffHigh then return
  if (sample > cutOffLow) {
    if (sample < cutOffHigh) {
      return sample;
    } 
  }

  //else return nothing
  return AMPLITUDE_PIVOT;
}

void printBandPassNoiseF(SFilter *filter) {

	printfToTerminal("Band-pass Noise Gate:\n\r\t\tCut Off Low: %f\n\r\t\tCut-off High: %f\n\r\r",
			filter->parameters[0], filter->parameters[1]);
}

Filter *createBandPassNoiseF(float cutOffLow, float cutOffHigh) {

	Filter *bandPassNoiseFilter = createFilterS(&bandPassNoiseF, &printBandPassNoiseF, cutOffLow, cutOffHigh,
			UNUSED, UNUSED, UNUSED);

	return bandPassNoiseFilter;
}
