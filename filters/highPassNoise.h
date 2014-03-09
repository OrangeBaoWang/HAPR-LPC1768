// Created by Dariush Bahri - 26/02/2014

#ifndef __HIGHPASSNOISE_H_
#define __HIGHPASSNOISE_H_

uint32_t highPassNoiseF(uint32_t sample, SFilter *filter);

void printHighPassNoiseF(SFilter *filter);

Filter *createHighPassNoiseF(float cutOff);

#endif
