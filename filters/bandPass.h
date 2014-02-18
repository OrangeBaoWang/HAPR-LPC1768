#ifndef __BANDPASS_H_
#define __BANDPASS_H_

uint32_t bandPassF(uint32_t sample, SFilter *filter);

void printBandPassF(SFilter *filter);

Filter *createBandPassF(float cutOffLow, float cutOffHigh);

#endif
