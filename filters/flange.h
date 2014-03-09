// Created by Oliver Lea - 31/01/2014

#ifndef __FLANGE_H_
#define __FLANGE_H_


uint32_t flangeF(uint32_t sample, SFilter *filter);

void printFlangeF(SFilter *filter);

Filter *createFlangeF(float mixingRatio, float range, float frequency);

#endif
