#include "lpc_types.h"
#include "lpc17xx_uart.h"

#include <stdlib.h>
#include <string.h>

#include "filter.h"
#include "filterChain.h"
#include "timerInit.h"

#include "filters/linearGain.h"
#include "filters/flange.h"
#include "filters/delay.h"
#include "filters/reverb.h"
#include "filters/envFollower.h"
#include "filters/echo.h"
#include "filters/tremelo.h"
#include "filters/overDrive.h"
#include "filters/lowPass.h"
#include "filters/highPass.h"
#include "filters/bandPass.h"

#include "debug.h"
#include "global.h"
#include "userInterface.h"

static uint8_t terminalBuffer; //buffer to store read values from terminal
static float filterVariable[5];

static uint32_t received; //flag to check if there has been a keyboard input
static uint8_t stay = 1; //stay to loop through switch statements

//recieves single char from keyboard input
uint32_t receiveFromTerminal(void) {
	return UART_Receive((LPC_UART_TypeDef *) LPC_UART0, &terminalBuffer, 1, NONE_BLOCKING);
}

//waits for keyboard input. loops until user inputs something
void waitForTerminal(void){
	while (!received){
		received = receiveFromTerminal();
	}
	received = 0;
}

void forceInput(void) {
	printToTerminal("\n\rPress any key to return to main menu...\n\r");
	waitForTerminal();
	clearScreen();
}

//takes string input from terminal converts and returns a float
float getFloat(void){
	char terminalArray[9];
	int index = 0;
	float inputFloat;
	
	while (terminalBuffer != 0x0D){ //while enter hasn't been pressed
		if (index > sizeof terminalArray){ //if end of input array
			printToTerminal("Input too long. Try again. \n\r");
			memset(terminalArray, 0, sizeof terminalArray);
			index = 0;
			waitForTerminal();
		}
		//if correct input character
		if (((terminalBuffer >= 0x30) && (terminalBuffer <= 0x39)) || terminalBuffer == 0x2E){ 
			terminalArray[index] = terminalBuffer;
			printfToTerminal("%c", terminalBuffer);
			index++; 
			waitForTerminal();		
		} else{
			printToTerminal("\n\rIncorrect input. Try again. \n\r");
			memset(terminalArray, 0, sizeof terminalArray);
			index = 0;
			waitForTerminal();
		}
	}

	terminalArray[index] = '\0'; //add null char at end so it recognises string
	index = 0;
	inputFloat = atof(terminalArray);
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
					"4 - Flange\n\r5 - Linear Gain\n\r6 - Reverb\n\r7 - Tremelo\n\r"
					"8 - Overdrive\n\r9 - Low-pass\n\r10 - High-pass\n\r"
					"11 - Band-pass\n\r\n\r");
	return;
}

void printUsage(void) {

	uint32_t usedTime;
	uint32_t usage;
	uint32_t remaining;

	usedTime = ((float) wdtCounter / (float) SAMPLE_RATE_US) * 100;
	usage = usedTime;

	printfToTerminal("WDT: %d", wdtCounter);

	printToTerminal("\n\rCPU Usage:  |");

	if (usedTime > 100) {
		while (usedTime > 100) {
			printToTerminal("=");
			usedTime -= 10;
		}
	}


	uint8_t i = 0;
	while (usage > 10 ) {

		usage -= 10;
		i++;
		printToTerminal("=");
	}

	printToTerminal("=>");

	remaining = 10;

	uint8_t y;
	for (y = 0; y < remaining; y++) {
		printToTerminal(" ");
	}

	printfToTerminal("|\t%d%%\n\r", usedTime);
}
	
void generateUI(void){
	
	while (1) {

		clearScreen();

		printToTerminal("\n\r################ MAIN MENU #################\n\r\n\r");

		//printUsage();

		if (passThrough) {
			printToTerminal("PASS-THROUGH ENABLED\n\r\n\r");
		}

		printToTerminal("1) Display all possible effects\n\r");
		printToTerminal("2) Display all added effects\n\r");
		printToTerminal("3) Remove effect\n\r");
		printToTerminal("4) Add effect\n\r");
		printToTerminal("5) Replace effect\n\r");
		printToTerminal("6) Enable/Disable pass-through\n\r");
		printToTerminal("7) Empty filter chain\n\r");
		printToTerminal("8) Exit \n\r\n\r");
		
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
				clearScreen();
				printQueue();
				printToTerminal("Enter index of effect to remove:\n\r");

				waitForTerminal();
				
				if (dequeueByIndex(getFloat()) == -1) {
					printToTerminal("Invalid index given\n\r");
				}

				forceInput();
				break;
			case 4:
				enqueueEffect();
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
				printToTerminal("\n\rRemoving all effects from the filter chain...");

				if (dequeueAll() == -1) {
					printToTerminal("Filter chain already empty\n\r");
				} else {
					printToTerminal("COMPLETE\n\r");
				}

				forceInput();
				break;
			case 8:
				printToTerminal("\n\rSystem will now terminate");
				disableTimer();
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

	printToTerminal("\n\rPlease choose the type of effect desired:\n\r"
					"1: Serial\n\r2: Parallel\n\r");

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
	printToTerminal("Enter index of effect to replace:\n\r");

	waitForTerminal();
	index = getFloat() - 1;

	if (dequeueByIndex(index+1) == -1) {
		printToTerminal("\n\rModification failed - Invalid dequeue index given\n\r");
		forceInput();
		return;
	}

	printToTerminal("\n\rPlease choose the type of effect desired:\n\r"
					"1: Serial\n\r2: Parallel\n\r");

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
				return inputReverb();
				break;
			case 7:
				return inputTremelo();
				break;
			case 8:
				return inputOverdrive();
				break;
			case 9:
				return inputLowPass();
				break;
			case 10:
				return inputHighPass();
				break;
			case 11:
				return inputBandPass();
				break;
			default:
				printToTerminal("\n\rEnter a correct effect number:\n\r");
				break;
		}
	}
}

Filter *inputDelay(void) {
	printToTerminal("\n\rEnter the size of the delay (0-8000):\n\r");
	filterVariable[0] = inputAndAssert(0, 8000);

	return createDelayF(filterVariable[0]);
}

Filter *inputEcho(void) {
	printToTerminal("\n\rEnter the mixing ratio (0-1):\n\r");
	filterVariable[0] = inputAndAssert(0, 1);

	printToTerminal("\n\rEnter the delay (0-8000):\n\r");
	filterVariable[1] = inputAndAssert(0, 8000);

	return createEchoF(filterVariable[0], filterVariable[1]);
}

Filter *inputEnvFollower(void) {
	printToTerminal("\n\rEnter the attack (ms):\n\r");
	filterVariable[0] = inputAndAssert(0, 10000);

	printToTerminal("\n\rEnter the release (ms):\n\r");
	filterVariable[1] = inputAndAssert(0, 10000);

	return createEnvFollowerF(filterVariable[0], filterVariable[1]);
}

Filter *inputFlange(void) {
	printToTerminal("\n\rEnter the mixing ratio (0-1):\n\r");
	filterVariable[0] = inputAndAssert(0, 1);

	printToTerminal("\n\rEnter the range of the sweep (0-8000):\n\r");
	filterVariable[1] = inputAndAssert(0, 8000);

	printToTerminal("\n\rEnter the frequency of the sweep (Hz)\n\r");
	filterVariable[2] = inputAndAssert(0, 10000);

	return createFlangeF(filterVariable[0], filterVariable[1], filterVariable[2]);
}

Filter *inputLinearGain(void) {
	printToTerminal("\n\rEnter the magnitude of the gain (eg. 0.4 or 1.2) (0-3):\n\r");
	filterVariable[0] = inputAndAssert(0, 3);

	return createLinearGainF(filterVariable[0]);
}

Filter *inputReverb(void) {
	printToTerminal("\n\rEnter the mixing ratio (0-1):\n\r");
	filterVariable[0] = inputAndAssert(0, 1);

	printToTerminal("\n\rEnter the delay (0-8000):\n\r");
	filterVariable[1] = inputAndAssert(0, 8000);

	return createReverbF(filterVariable[0], filterVariable[1]);
}

Filter *inputTremelo(void) {
	printToTerminal("\n\rEnter the range of the sweep (0-1):\n\r");
	filterVariable[0] = inputAndAssert(0, 1);

	printToTerminal("\n\rEnter the frequency of the sweep (Hz):\n\r");
	filterVariable[1] = inputAndAssert(0, 10000);

	return createTremeloF(filterVariable[0], filterVariable[1]);
}

Filter *inputOverdrive(void) {
	printToTerminal("\n\rEnter the boost (0-100):\n\r");
	filterVariable[0] = inputAndAssert(0, 100);
/*
	printToTerminal("\n\rEnter the input drive (0-100):\n\r");
	filterVariable[1] = inputAndAssert(0, 100);

	printToTerminal("\n\rEnter the drive scalar (0-1):\n\r");
	filterVariable[2] = inputAndAssert(0, 1);
*/
	return createOverdriveF(filterVariable[0]);
}

Filter *inputLowPass(void) {
	printToTerminal("\n\rEnter the cutoff amplitude (0-4000):\n\r");
	filterVariable[0] = inputAndAssert(0, 4000);

	return createLowPassF(filterVariable[0]);
}

Filter *inputHighPass(void) {
	printToTerminal("\n\rEnter the cutoff amplitude (0-4000):\n\r");
	filterVariable[0] = inputAndAssert(0, 4000);

	return createHighPassF(filterVariable[0]);
}

Filter *inputBandPass(void) {
	printToTerminal("\n\rEnter the bottom cutoff amplitude (0-4000):\n\r");
	filterVariable[0] = inputAndAssert(0, 4000);

	printToTerminal("\n\rEnter the top cutoff amplitude (0-4000):\n\r");
	filterVariable[1] = inputAndAssert(0, 4000);

	return createBandPassF(filterVariable[0], filterVariable[1]);
}