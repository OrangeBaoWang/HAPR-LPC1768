#ifndef __REVERB_H__
#define __REVERB_H__

uint32_t reverbF(uint32_t sample, float parameters[5]);

void printReverbF(float parameters[5]);

Filter *createReverbF(float mixingRatio, float delay);

#endif
