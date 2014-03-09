// Created by Dariush Bahri - 04/02/2014

// O-Edit: Added inputAndAssert() function that will force the user to input a value
//	within the range specified by the parameters of inputAndAssert() - 12/02/2014

// O-Edit: Added forceInput() function that tells the user to press any key in order
//	to return to the main menu - 13/02/2014

// O-Edit: Refactored the entire UI in order to reduce repeated code. Also added a new
//	getEffect() function that gets the choice of effect from the user and creates
//	a Filter struct from the choice - 21/02/2014

// O-Edit: Added a work in progress printUsage() function that should tell the user
//	how hard the CPU is working, and the current sample rate - 25/02/2014

#ifndef __USERINTERFACE_H__
#define __USERINTERFACE_H__

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

Filter *inputOverdrive(void);

Filter *inputReverb(void);

Filter *inputTremelo(void);

Filter *inputLowPassNoise(void);

Filter *inputHighPassNoise(void);

Filter *inputBandPassNoise(void);

#endif
