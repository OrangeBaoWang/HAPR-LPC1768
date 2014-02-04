#ifndef __TREMELO_H__
#define __TREMELO_H__


uint32_t tremeloF(uint32_t sample, float parameters[5]);

void printTremeloF(float parameters[5]);

Filter *createTremeloF(float range, float frequency);

#endif
