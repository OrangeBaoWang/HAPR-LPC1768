// Created by Oliver Lea - 31/01/2014

#ifndef __ECHO_H_
#define __ECHO_H_

uint32_t echoF(uint32_t sample, SFilter *filter);

void printEchoF(SFilter *filter);

Filter *createEchoF(float mixingRatio, float delay);

#endif
