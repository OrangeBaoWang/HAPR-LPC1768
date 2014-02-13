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
	printToTerminal("1 - Delay\n\r2 - Echo\n\r3 - Enveloper Follower\n\r"
					"4 - Flange\n\r5 - Linear Gain\n\r6 - Reverb\n\r7 - Tremelo\n\r");
	return;
}
	
void generateUI(void){

	clearScreen();
	
	while (1){

		printToTerminal("\n\r################ MAIN MENU #################\n\r");
		printToTerminal("1) Display all possible effects\n\r");
		printToTerminal("2) Display all added effects\n\r");
		printToTerminal("3) Remove effects\n\r");
		printToTerminal("4) Add effects\n\r");
		printToTerminal("5) Exit \n\r\n\r");
		
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
				dequeueByIndex(getFloat());

				// ADD SUCCESS OR FAILURE DEPENDING ON WHETHER INDEX EXISTS
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
							printToTerminal("Enter the size of the delay (0-8000):\n\r");
							filterVariable[0] = inputAndAssert(0, 8000);

							enqueue(createDelayF(filterVariable[0]));
							stay = 0;
							break;
						case '2':
							printToTerminal("Enter the mixing ratio (0-1):\n\r");
							filterVariable[0] = inputAndAssert(0, 1);

							printToTerminal("\n\rEnter the delay (0-8000):\n\r");
							filterVariable[1] = inputAndAssert(0, 8000);

							enqueue(createEchoF(filterVariable[0], filterVariable[1]));
							stay = 0;
							break;
						case '3':
							printToTerminal("Enter the attack (ms):\n\r");
							filterVariable[0] = inputAndAssert(0, 10000);

							printToTerminal("\n\rEnter the release (ms):\n\r");
							filterVariable[1] = inputAndAssert(0, 10000);

							enqueue(createEnvFollowerF(filterVariable[0], filterVariable[1]));
							stay = 0;
							break;
						case '4':
							printToTerminal("Enter the mixing ratio (0-1):\n\r");
							filterVariable[0] = inputAndAssert(0, 1);

							printToTerminal("\n\rEnter the range of the sweep (0-8000):\n\r");
							filterVariable[1] = inputAndAssert(0, 8000);

							printToTerminal("\n\rEnter the frequency of the sweep (Hz)");
							filterVariable[2] = inputAndAssert(0, 10000);

							enqueue(createFlangeF(filterVariable[0], filterVariable[1], filterVariable[2]));
							stay = 0;
							break;
						case '5':
							printToTerminal("Enter the magnitude of the gain (eg. 0.4 or 1.2) (0-3):\n\r");
							filterVariable[0] = inputAndAssert(0, 3);

							enqueue(createLinearGainF(filterVariable[0]));
							stay = 0;
							break;
						case '6':
							printToTerminal("Enter the mixing ratio (0-1):\n\r");
							filterVariable[0] = inputAndAssert(0, 1);

							printToTerminal("\n\rEnter the delay (0-8000):\n\r");
							filterVariable[1] = inputAndAssert(0, 8000);

							enqueue(createReverbF(filterVariable[0], filterVariable[1]));
							stay = 0;
							break;
						case '7':
							printToTerminal("Enter the range of the sweep (0-1):\n\r");
							filterVariable[0] = inputAndAssert(0, 1);

							printToTerminal("\n\rEnter the frequency of the sweep (Hz)");
							filterVariable[1] = inputAndAssert(0, 10000);

							enqueue(createTremeloF(filterVariable[0], filterVariable[1]));
							stay = 0;
							break;
						default:
							printToTerminal("Enter a correct effect number\n\r");
							stay = 1;
							break;
					}
				}
				break;
				
			case '5':
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


