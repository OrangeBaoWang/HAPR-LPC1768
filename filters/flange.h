#ifndef __FLANGE_H_
#define __FLANGE_H_


uint32_t flangeF(uint32_t sample, float parameters[5]);

void printFlangeF(float parameters[5]);

Filter *createFlangeF(float mixingRatio, float range, float sineIncrement);

#endif
