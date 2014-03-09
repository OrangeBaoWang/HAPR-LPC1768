// Created by Oliver Lea - 31/01/2014

#ifndef __DELAY_H_
#define __DELAY_H_

uint32_t delayF(uint32_t sample, SFilter *filter);

void printDelayF(SFilter *filter);

Filter *createDelayF(float delay);

#endif
