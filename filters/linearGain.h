#ifndef __LINEARGAIN_H_
#define __LINEARGAIN_H_

uint32_t linearGainF(uint32_t sample, float parameters[5]);

void printLinearGainF(float parameters[5]);

Filter *createLinearGainF(float gain);

#endif
