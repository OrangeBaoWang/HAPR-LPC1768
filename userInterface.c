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
					"4 - Flange\n\r5 - Linear Gain\n\r6 - Reverb\n\r7 - Tremelo\n\r\n\r");
	return;
}
	
void generateUI(void){

	uint8_t testEnqueue;

	// A float for use when dequeueing and enqueueing by an index
	float index;
	
	while (1) {

		clearScreen();

		printToTerminal("\n\r################ MAIN MENU #################\n\r");
		printToTerminal("1) Display all possible effects\n\r");
		printToTerminal("2) Display all added effects\n\r");
		printToTerminal("3) Remove effect\n\r");
		printToTerminal("4) Add effect\n\r");
		printToTerminal("5) Replace effect\n\r");
		printToTerminal("6) Empty chain (pass-through");
		printToTerminal("7) Exit \n\r\n\r");
		
		waitForTerminal();
		switch (terminalBuffer) {
			case '1':
				clearScreen();
				printEffects();
				forceInput();
				break;
			case '2':
				clearScreen();
				printQueue();
				forceInput();
				break;
			case '3':
				clearScreen();
				printQueue();
				printToTerminal("Enter index of effect to remove:\n\r");

				waitForTerminal();
				
				if (dequeueByIndex(getFloat()) == -1) {
					printToTerminal("Invalid index given\n\r");
				}

				forceInput();
				break;
			case '4':
				clearScreen();
				printToTerminal("Enter number of effect to add:\n\r");
				printEffects();

				while (stay){
					waitForTerminal();
					switch(terminalBuffer){
						case '1':
							inputDelay();

							enqueue(createDelayF(filterVariable[0]));
							stay = 0;
							break;
						case '2':
							inputEcho();

							enqueue(createEchoF(filterVariable[0], filterVariable[1]));
							stay = 0;
							break;
						case '3':
							inputEnvFollower();

							enqueue(createEnvFollowerF(filterVariable[0], filterVariable[1]));
							stay = 0;
							break;
						case '4':
							inputFlange();

							enqueue(createFlangeF(filterVariable[0], filterVariable[1], filterVariable[2]));
							stay = 0;
							break;
						case '5':
							inputLinearGain();

							enqueue(createLinearGainF(filterVariable[0]));
							stay = 0;
							break;
						case '6':
							inputReverb();

							enqueue(createReverbF(filterVariable[0], filterVariable[1]));
							stay = 0;
							break;
						case '7':
							inputTremelo();

							enqueue(createTremeloF(filterVariable[0], filterVariable[1]));
							stay = 0;
							break;
						default:
							printToTerminal("Enter a correct effect number:\n\r");
							stay = 1;
							break;
					}
				}

				forceInput();
				break;

			case '5':
				clearScreen();
				printQueue();
				printToTerminal("Enter index of effect to replace:\n\r");

				waitForTerminal();
				index = getFloat() - 1;

				if (dequeueByIndex(index+1) == -1) {
					printToTerminal("Modification failed - Invalid dequeue index given\n\r");
					forceInput();
					break;
				}

				printEffects();
				printToTerminal("Enter number of effect to add:\n\r");
				
				while (stay){
					waitForTerminal();
					switch(terminalBuffer){
						case '1':
							inputDelay();

							testEnqueue = enqueueByIndex(createDelayF(filterVariable[0]), index);
							stay = 0;
							break;
						case '2':
							inputEcho();

							testEnqueue = enqueueByIndex(createEchoF(filterVariable[0], filterVariable[1]), index);
							stay = 0;
							break;
						case '3':
							inputEnvFollower();

							testEnqueue = enqueueByIndex(createEnvFollowerF(filterVariable[0], filterVariable[1]), index);
							stay = 0;
							break;
						case '4':
							inputFlange();

							testEnqueue = enqueueByIndex(createFlangeF(filterVariable[0], filterVariable[1], filterVariable[2]), index);
							stay = 0;
							break;
						case '5':
							inputLinearGain();

							testEnqueue = enqueueByIndex(createLinearGainF(filterVariable[0]), index);
							stay = 0;
							break;
						case '6':
							inputReverb();

							testEnqueue = enqueueByIndex(createReverbF(filterVariable[0], filterVariable[1]), index);
							stay = 0;
							break;
						case '7':
							inputTremelo();

							testEnqueue = enqueueByIndex(createTremeloF(filterVariable[0], filterVariable[1]),index);
							stay = 0;
							break;
						default:
							printToTerminal("Enter a correct effect number:\n\r");
							stay = 1;
							break;
					}
				}
				if (testEnqueue != 0) {
					printToTerminal("Modification failed - Invalid enqueue given\n\r");
				}

				forceInput();
				break;
			case '6':
				printToTerminal("Removing all effects from the filter chain...");
				
				if (dequeueAll() == -1) {
					printToTerminal("Filter chain already empty\n\r");
				} else {
					printToTerminal("COMPLETE\n\r");
				}

				forceInput();
				break;
			case '7':
				printToTerminal("System will now terminate");
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

void inputDelay(void) {
	printToTerminal("Enter the size of the delay (0-8000):\n\r");
	filterVariable[0] = inputAndAssert(0, 8000);
}

void inputEcho(void) {
	printToTerminal("Enter the mixing ratio (0-1):\n\r");
	filterVariable[0] = inputAndAssert(0, 1);

	printToTerminal("\n\rEnter the delay (0-8000):\n\r");
	filterVariable[1] = inputAndAssert(0, 8000);
}

void inputEnvFollower(void) {
	printToTerminal("Enter the attack (ms):\n\r");
	filterVariable[0] = inputAndAssert(0, 10000);

	printToTerminal("\n\rEnter the release (ms):\n\r");
	filterVariable[1] = inputAndAssert(0, 10000);
}

void inputFlange(void) {
	printToTerminal("Enter the mixing ratio (0-1):\n\r");
	filterVariable[0] = inputAndAssert(0, 1);

	printToTerminal("\n\rEnter the range of the sweep (0-8000):\n\r");
	filterVariable[1] = inputAndAssert(0, 8000);

	printToTerminal("\n\rEnter the frequency of the sweep (Hz)\n\r");
	filterVariable[2] = inputAndAssert(0, 10000);
}

void inputLinearGain(void) {
	printToTerminal("Enter the magnitude of the gain (eg. 0.4 or 1.2) (0-3):\n\r");
	filterVariable[0] = inputAndAssert(0, 3);
}

void inputReverb(void) {
	printToTerminal("Enter the mixing ratio (0-1):\n\r");
	filterVariable[0] = inputAndAssert(0, 1);

	printToTerminal("\n\rEnter the delay (0-8000):\n\r");
	filterVariable[1] = inputAndAssert(0, 8000);
}

void inputTremelo(void) {
	printToTerminal("Enter the range of the sweep (0-1):\n\r");
	filterVariable[0] = inputAndAssert(0, 1);

	printToTerminal("\n\rEnter the frequency of the sweep (Hz)");
	filterVariable[1] = inputAndAssert(0, 10000);
}

