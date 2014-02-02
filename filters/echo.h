#ifndef __ECHO_H_
#define __ECHO_H_

uint32_t echoF(uint32_t sample, float parameters[5]);

void printEchoF(float parameters[5]);

Filter *createEchoF(float mixingRatio, float delay);

#endif
