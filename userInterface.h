#ifndef __USERINTERFACE_H__
#define __USERINTERFACE_H__

uint32_t receiveFromTerminal(void);

void waitForTerminal(void);

void forceInput(void);

float getFloat(void);

void printEffects(void);

void generateUI(void);

void enqueueEffect(void);

void replaceEffect(void);

Filter *getEffect(void);

Filter *inputDelay(void);

Filter *inputEcho(void);

Filter *inputEnvFollower(void);

Filter *inputFlange(void);

Filter *inputLinearGain(void);

Filter *inputReverb(void);

Filter *inputTremelo(void);

Filter *inputOverdrive(void);

Filter *inputLowPassNoise(void);

Filter *inputHighPassNoise(void);

Filter *inputBandPassNoise(void);

#endif
