#include "lpc_types.h"

#include "../filter.h"
#include "bandPass.h"
#include "../global.h"
#include "../debug.h"

uint32_t bandPassF(uint32_t sample, SFilter *filter) {
  
  float cutOffLow = filter->parameters[0];
  float cutOffHigh = filter->parameters[1];
  
  //if sample is above cutOffLow and below cutOffHigh then return
  if (sample > cutOffLow) {
    if (sample < cutOffHigh) {
      return sample;
    } 
  }

  //else return nothing
  return 0;
}

void printBandPassF(SFilter *filter) {

	printfToTerminal("Band Pass:\n\r\t\tCut Off Low: %f\n\r\r %f\n\r\t\tCut Off High: %f\n\r\r",
			filter->parameters[0], filter->parameters[1]);
}

Filter *createBandPassF(float cutOffLow, float cutOffHigh) {

	Filter *bandPassFilter = createFilterS(&bandPassF, &printBandPassF, cutOffLow, cutOffHigh,
			UNUSED, UNUSED, UNUSED);

	return bandPassFilter;
}
