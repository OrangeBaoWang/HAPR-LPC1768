// Created by Oliver Lea - 04/02/2014

#ifndef __TREMELO_H__
#define __TREMELO_H__


uint32_t tremeloF(uint32_t sample, SFilter *filter);

void printTremeloF(SFilter *filter);

Filter *createTremeloF(float range, float frequency);

#endif
