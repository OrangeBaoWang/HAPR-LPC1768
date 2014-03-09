// Created by Oliver Lea - 31/01/2014

#ifndef __LINEARGAIN_H_
#define __LINEARGAIN_H_

uint32_t linearGainF(uint32_t sample, SFilter *filter);

void printLinearGainF(SFilter *filter);

Filter *createLinearGainF(float gain);

#endif
