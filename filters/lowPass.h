#ifndef __LOWPASS_H_
#define __LOWPASS_H_

uint32_t lowPassF(uint32_t sample, SFilter *filter);

void printLowPassF(SFilter *filter);

Filter *createLowPassF(float cutOff);

#endif
