// Created by James Lee - 13/02/2014

#ifndef __OVERDRIVE_H_
#define __OVERDRIVE_H_

uint32_t overdriveF(uint32_t sample, SFilter *filter);

void printOverdriveF(SFilter *filter);

Filter *createOverdriveF(float boost);

#endif
