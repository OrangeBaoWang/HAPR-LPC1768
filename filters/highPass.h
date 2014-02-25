#ifndef __HIGHPASS_H_
#define __HIGHPASS_H_

uint32_t highPassF(uint32_t sample, SFilter *filter);

void printHighPassF(SFilter *filter);

Filter *createHighPassF(float cutOff);

#endif
