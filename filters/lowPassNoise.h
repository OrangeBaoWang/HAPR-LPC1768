// Created by Dariush Bahri - 26/02/2014

#ifndef __LOWPASSNOISE_H_
#define __LOWPASSNOISE_H_

uint32_t lowPassNoiseF(uint32_t sample, SFilter *filter);

void printLowPassNoiseF(SFilter *filter);

Filter *createLowPassNoiseF(float cutOff);

#endif
