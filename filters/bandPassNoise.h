// Created by Dariush Bahri - 26/02/2014

#ifndef __BANDPASSNOISE_H_
#define __BANDPASSNOISE_H_

uint32_t bandPassNoiseF(uint32_t sample, SFilter *filter);

void printBandPassNoiseF(SFilter *filter);

Filter *createBandPassNoiseF(float cutOffLow, float cutOffHigh);

#endif
