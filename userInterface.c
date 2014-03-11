// Created by Dariush Bahri - 04/02/2014

// J-Edit: Added standard UI menu code - 04/02/2014

// O-Edit: Added inputAndAssert() function that will force the user to input a value
//	within the range specified by the parameters of inputAndAssert() - 12/02/2014

// O-Edit: Added forceInput() function that tells the user to press any key in order
//	to return to the main menu - 13/02/2014

// J-Edit: Added support in the UI for emptying the filter chain using the dequeueAll()
//	function - 18/02/2014

// O-Edit: Changed the switch statements in the UI to switch on floats instead of chars
//	in order to support more than 10 cases - 18/02/2014

// O-Edit: Refactored the entire UI in order to reduce repeated code. Also added a new
//	getEffect() function that gets the choice of effect from the user and creates
//	a Filter struct from the choice - 21/02/2014

// O-Edit: Added support in the UI for pass-through - 24/02/2014

// J-Edit: Tidied code for readability with new functions - 25/02/2014

// D-Edit: Added support for the unfinished infrared mixing - 25/02/2014

// O-Edit: CPU usage support now complete. General UI cleaning - 26/02/2014

// O-Edit: Added recommended parameters for input to filters in the UI - 04/03/2014

#include "lpc_types.h"
#include "lpc17xx_uart.h"

#include <stdlib.h>
#include <string.h>

#include "filter.h"
#include "filterChain.h"
#include "timerInit.h"
#include "watchdog.h"

#include "filters/linearGain.h"
#include "filters/flange.h"
#include "filters/delay.h"
#include "filters/reverb.h"
#include "filters/envFollower.h"
#include "filters/echo.h"
#include "filters/tremelo.h"
#include "filters/overDrive.h"
#include "filters/lowPassNoise.h"
#include "filters/highPassNoise.h"
#include "filters/bandPassNoise.h"

#include "debug.h"
#include "global.h"
#include "userInterface.h"

static float filterVariable[5];

// Boolean used to decide when to show CPU usage in the main UI loop
static uint8_t showUsage = 0;

static uint8_t stay = 1; //stay to loop through switch statements

void forceInput(void) {
	printToTerminal("\n\rPress any key to return to main menu...\n\r");
	waitForTerminal();
	clearScreen();
}

//takes string input from terminal, converts and returns a float
float getFloat(void) {
	char terminalArray[9];
	int index = 0;
	float inputFloat;
	
	while (terminalBuffer != 0x0D) { //while enter hasn't been pressed
		if (index > sizeof terminalArray) { //if end of input array
			printToTerminal("\n\rInput too long - try again:\n\r");
			memset(terminalArray, 0, sizeof terminalArray);
			index = 0;
			waitForTerminal();
		}
		//if correct input character
		if (((terminalBuffer >= 0x30) && (terminalBuffer <= 0x39)) || terminalBuffer == 0x2E) { 
			terminalArray[index] = terminalBuffer;
			printfToTerminal("%c", terminalBuffer);
			index++; 
			waitForTerminal();		
		} else {
			printToTerminal("\n\rIncorrect input - try again:\n\r");
			memset(terminalArray, 0, sizeof terminalArray);
			index = 0;
			waitForTerminal();
		}
	}

	terminalArray[index] = '\0'; //add null char at end so it recognises string
	index = 0;
	inputFloat = atof(terminalArray);

	// Ensures there is always a space between the input of a
	// number and the next line printed
	printToTerminal("\n\r");

	return inputFloat; //convert char array to float, return
}

// Will force the user to input a valid number for the given context
float inputAndAssert(float min, float max) {

	float input;

	while (1) {

		waitForTerminal();
		input = getFloat();

		if (input >= min) {
			if (input <= max) {
				break;
			}
		}
		printfToTerminal("You must enter a number between %f and %f\n\r", min, max);
	}

	return input;
}

void printEffects(void) {
	printToTerminal("\n\r1 - Delay\n\r2 - Echo\n\r3 - Enveloper Follower\n\r"
					"4 - Flange\n\r5 - Linear Gain\n\r6 - Overdrive\n\r"
					"7 - Reverb\n\r8 - Tremelo\n\r9 - Low-pass Noise Gate\n\r10 - High-pass Noise Gate\n\r"
					"11 - Band-pass Noise Gate\n\r\n\r");
	return;
}
	
void generateUI(void) {
	
	while (1) {

		clearScreen();

		printToTerminal("\n\r################ MAIN MENU #################\n\r\n\r");

		if (showUsage) {
			printUsage();
		}
		if (passThrough) {
			printToTerminal("PASS-THROUGH ENABLED\n\r\n\r");
		}
		if (infraMix) {
			printToTerminal("INFRARED MIX ENABLED\n\r\n\r");
    		}

		printToTerminal("1)\tDisplay all possible effects\n\r");
		printToTerminal("2)\tDisplay all added effects\n\r\n\r");

		printToTerminal("3)\tAdd effect\n\r");
		printToTerminal("4)\tRemove effect\n\r");
		printToTerminal("5)\tReplace effect\n\r\n\r");

		printToTerminal("6)\tEnable/disable pass-through\n\r");
		printToTerminal("7)\tEnable/disable infrared mix\n\r\n\r");

		printToTerminal("8)\tShow/hide CPU usage\n\r");
		printToTerminal("9)\tEmpty filter chain\n\r");
		printToTerminal("10)\tExit \n\r\n\r");
		
		waitForTerminal();
		switch ((uint32_t) getFloat()) {
			case 1:
				clearScreen();
				printEffects();
				forceInput();
				break;
			case 2:
				clearScreen();
				printQueue();
				forceInput();
				break;
			case 3:
				enqueueEffect();
				break;
			case 4:
				clearScreen();
				printQueue();
				printToTerminal("Enter index of effect to remove:\n\r");

				waitForTerminal();
				
				if (dequeueByIndex(getFloat()) == -1) {
					printToTerminal("Invalid index given\n\r");
				}

				forceInput();
				break;
			case 5:
				replaceEffect();
				break;
			case 6:
				if (passThrough) {
					passThrough = 0;
					printToTerminal("\n\rPass-through disabled\n\r");
				} else {
					passThrough = 1;
					printToTerminal("\n\rPass-through enabled\n\r");
				}

				forceInput();
				break;
			case 7:
				if (infraMix) {
					infraMix = 0;
					printToTerminal("\n\rInfrared mix disabled\n\r");
				} else {
					infraMix = 1;
					printToTerminal("\n\rInfrared mix enabled\n\r");
				}

				forceInput();
				break;
			case 8:
				if (showUsage) {
					showUsage = 0;
					printToTerminal("\n\rHiding CPU usage\n\r");
				} else {
					showUsage = 1;
					printToTerminal("\n\rShowing CPU usage\n\r");
				}
				forceInput();
				break;
			case 9:
				printToTerminal("\n\rRemoving all effects from the filter chain...");

				if (dequeueAll() == -1) {
					printToTerminal("Filter chain already empty\n\r");
				} else {
					printToTerminal("COMPLETE\n\r");
				}
				forceInput();
				break;
			case 10:
				printToTerminal("\n\rSystem terminating...");

				// Disable sampling timer
				disableTimer();

				// Disable WDT
				NVIC_DisableIRQ(WDT_IRQn);

				printToTerminal("COMPLETE");
				exit(0);
				break;
			default:
				printToTerminal("Enter a correct command\n\r");
				break;
		}
		stay = 1; //reset stay to loop through switch statements again
	}
}

void enqueueEffect(void) {

	float mixingRatio;

	uint8_t stay = 1;

	clearScreen();

	printToTerminal("\n\rPlease choose the type of effect to add:\n\r\n\r"
					"1:\tSerial\n\r2:\tParallel\n\r\n\r");

	while(stay) {
		waitForTerminal();
		switch((uint32_t) getFloat()) {
			case 1:
				// For serial effects
				enqueue(getEffect());

				stay = 0;
				break;
			case 2:
				// For parallel effects
				printToTerminal("\n\rPlease enter a mixing ratio:\n\r");
				mixingRatio = inputAndAssert(0, 1);

				enqueue(createFilterP(getEffect(), getEffect(), mixingRatio));
				stay = 0;
				break;
			default:
				printToTerminal("\n\rEnter a correct effect number:\n\r");
				stay = 1;
				break;
		}
	}

	forceInput();
}

void replaceEffect(void) {

	uint8_t stay = 1;
	uint8_t testEnqueue;

	float index;
	float mixingRatio;

	clearScreen();
	printQueue();
	printToTerminal("\n\rEnter index of effect to replace:\n\r");

	waitForTerminal();
	index = getFloat();

	if (dequeueByIndex(index) == -1) {
		printToTerminal("\n\rModification failed - Invalid dequeue index given\n\r");
		forceInput();
		return;
	}

	printToTerminal("\n\rPlease choose the type of effect to add:\n\r\n\r"
					"1:\tSerial\n\r2:\tParallel\n\r\n\r");

	while(stay) {
		waitForTerminal();
		switch((uint32_t) getFloat()) {
			case 1:
				// For serial effects
				testEnqueue = enqueueByIndex(getEffect(), index);

				stay = 0;
				break;
			case 2:
				// For parallel effects
				printToTerminal("Please enter a mixing ratio:\n\r");
				mixingRatio = inputAndAssert(0, 1);

				testEnqueue = enqueueByIndex(createFilterP(getEffect(), getEffect(), mixingRatio), index);
				stay = 0;
				break;
			default:
				printToTerminal("\n\rEnter a correct effect number:\n\r");
				stay = 1;
				break;
		}
	}

	forceInput();
	return;
}

Filter *getEffect(void) {

	clearScreen();
	printToTerminal("Enter number of effect to add:\n\r");
	printEffects();

	while (1){
		waitForTerminal();
		switch((uint32_t) getFloat()){
			case 1:
				return inputDelay();
				break;
			case 2:
				return inputEcho();
				break;
			case 3:
				return inputEnvFollower();
				break;
			case 4:
				return inputFlange();
				break;
			case 5:
				return inputLinearGain();
				break;
			case 6:
				return inputOverdrive();
				break;
			case 7:
				return inputReverb();
				break;
			case 8:
				return inputTremelo();
				break;
			case 9:
				return inputLowPassNoise();
				break;
			case 10:
				return inputHighPassNoise();
				break;
			case 11:
				return inputBandPassNoise();
				break;
			default:
				printToTerminal("\n\rEnter a correct effect number:\n\r");
				break;
		}
	}
}

Filter *inputDelay(void) {

	printToTerminal("\n\rRecommended:\n\r\t\tDelay: 8000\n\r");

	printToTerminal("\n\rEnter the size of the delay (0-8000):\n\r");
	filterVariable[0] = inputAndAssert(0, 8000);

	return createDelayF(filterVariable[0]);
}

Filter *inputEcho(void) {

	printToTerminal("\n\rRecommended:\n\r"
			"\t\tNoticeable:\n\r\t\t\t\tMixing Ratio: 0.5\n\r\t\t\t\tDelay: 8000\n\r"
			"\t\tChoral:\n\r\t\t\t\tMixingRatio: 0.5\n\r\t\t\t\tDelay: 1500\n\r");

	printToTerminal("\n\rEnter the mixing ratio (0-1):\n\r");
	filterVariable[0] = inputAndAssert(0, 1);

	printToTerminal("\n\rEnter the delay (0-8000):\n\r");
	filterVariable[1] = inputAndAssert(0, 8000);

	return createEchoF(filterVariable[0], filterVariable[1]);
}

// Ideal attack is 2 (ms) and ideal release is 2 (ms)
Filter *inputEnvFollower(void) {

	printToTerminal("\n\rRecommended:\n\r\t\tAttack: 2 (ms)\n\r\t\tRelease: 2 (ms)\n\r");

	printToTerminal("\n\rEnter the attack (ms):\n\r");
	filterVariable[0] = inputAndAssert(0, 10000);

	printToTerminal("\n\rEnter the release (ms):\n\r");
	filterVariable[1] = inputAndAssert(0, 10000);

	return createEnvFollowerF(filterVariable[0], filterVariable[1]);
}

Filter *inputFlange(void) {

	printToTerminal("\n\rRecommended:\n\r\t\t\t\tMixingRatio: 0.4\n\r"
			"\t\tWub:\n\r\t\t\t\tSweep Range: 500\n\r\t\t\t\tSweep Frequency: 20\n\r"
			"\t\tUFO:\n\r\t\t\t\tSweep Range: 8000\n\r\t\t\t\tSweep Frequency: 1\n\r"
			"\t\tChoral:\n\r\t\t\t\tSweep Range: 2000\n\r\t\t\t\tSweep Frequency: 0.1\n\r");

	printToTerminal("\n\rEnter the mixing ratio (0-1):\n\r");
	filterVariable[0] = inputAndAssert(0, 1);

	printToTerminal("\n\rEnter the range of the sweep (0-8000):\n\r");
	filterVariable[1] = inputAndAssert(0, 8000);

	printToTerminal("\n\rEnter the frequency of the sweep (Hz)\n\r");
	filterVariable[2] = inputAndAssert(0, 10000);

	return createFlangeF(filterVariable[0], filterVariable[1], filterVariable[2]);
}

Filter *inputLinearGain(void) {

	printToTerminal("\n\rRecommended:\n\r\t\t\t\tGain Magnitude: 1.1-1.2\n\r");

	printToTerminal("\n\rEnter the magnitude of the gain (0-3):\n\r");
	filterVariable[0] = inputAndAssert(0, 3);

	return createLinearGainF(filterVariable[0]);
}

Filter *inputOverdrive(void) {

	printToTerminal("\n\rRecommended:\n\r\t\tBoost magnitude: 1.9\n\r");

	printToTerminal("\n\rEnter the boost magnitude (1.5-2):\n\r");
	filterVariable[0] = inputAndAssert(1.5, 2);

	return createOverdriveF(filterVariable[0]);
}

Filter *inputReverb(void) {

	printToTerminal("\n\rRecommended:\n\r\t\t\t\tMixing Ratio: 0.4\n\r\t\t\t\tDelay: 4000\n\r");

	printToTerminal("\n\rEnter the mixing ratio (0-1):\n\r");
	filterVariable[0] = inputAndAssert(0, 1);

	printToTerminal("\n\rEnter the delay (0-8000):\n\r");
	filterVariable[1] = inputAndAssert(0, 8000);

	return createReverbF(filterVariable[0], filterVariable[1]);
}

Filter *inputTremelo(void) {

	printToTerminal("\n\rRecommended:\n\r\t\t\t\tSweep Range: 0.1\n\r\t\t\t\tSweep Frequency: 10\n\r");

	printToTerminal("\n\rEnter the range of the sweep (0-1):\n\r");
	filterVariable[0] = inputAndAssert(0, 1);

	printToTerminal("\n\rEnter the frequency of the sweep (Hz):\n\r");
	filterVariable[1] = inputAndAssert(0, 10000);

	return createTremeloF(filterVariable[0], filterVariable[1]);
}

Filter *inputLowPassNoise(void) {

	printfToTerminal("\n\rAmplitude pivot is %d\n\r", AMPLITUDE_PIVOT);

	printToTerminal("\n\rEnter the cutoff amplitude (0-4000):\n\r");
	filterVariable[0] = inputAndAssert(0, 4000);

	return createLowPassNoiseF(filterVariable[0]);
}

Filter *inputHighPassNoise(void) {

	printfToTerminal("\n\rAmplitude pivot is %d\n\r", AMPLITUDE_PIVOT);

	printToTerminal("\n\rEnter the cutoff amplitude (0-4000):\n\r");
	filterVariable[0] = inputAndAssert(0, 4000);

	return createHighPassNoiseF(filterVariable[0]);
}

Filter *inputBandPassNoise(void) {

	printfToTerminal("\n\rAmplitude pivot is %d\n\r", AMPLITUDE_PIVOT);

	printToTerminal("\n\rEnter the bottom cutoff amplitude (0-4000):\n\r");
	filterVariable[0] = inputAndAssert(0, 4000);

	printToTerminal("\n\rEnter the top cutoff amplitude (0-4000):\n\r");
	filterVariable[1] = inputAndAssert(0, 4000);

	return createBandPassNoiseF(filterVariable[0], filterVariable[1]);
}
