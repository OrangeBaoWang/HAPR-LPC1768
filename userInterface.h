#ifndef __USERINTERFACE_H__
#define __USERINTERFACE_H__

uint32_t receiveFromTerminal(void);

void waitForTerminal(void);

void forceInput(void);

float getFloat(void);

void printEffects(void);

void generateUI(void);

void inputDelay(void);

void inputEcho(void);

void inputEnvFollower(void);

void inputFlange(void);

void inputLinearGain(void);

void inputReverb(void);

void inputTremelo(void);

#endif
