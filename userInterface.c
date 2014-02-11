#include "lpc_types.h"
#include "lpc17xx_uart.h"

#include <stdlib.h>
#include <string.h>

#include "filter.h"
#include "filterChain.h"

#include "filters/linearGain.h"
#include "filters/flange.h"
#include "filters/delay.h"
#include "filters/reverb.h"
#include "filters/envFollower.h"
#include "filters/echo.h"

#include "debug.h"
#include "global.h"
#include "userInterface.h"

uint8_t terminalBuffer; //buffer to store read values from terminal
float filterVariable;

uint32_t received; //flag to check if there has been a keyboard input
int flag = 1; //flag to loop through switch statements

//recieves single char from keyboard input
uint32_t receiveFromTerminal() {
	return UART_Receive((LPC_UART_TypeDef *) LPC_UART0, &terminalBuffer, 1, NONE_BLOCKING);
}

//waits for keyboard input. loops until user inputs something
void waitForTerminal(){
	while (!received){
		received = receiveFromTerminal();
	}
	received = 0;
}

//takes string input from terminal converts and returns a float
float getFloat(){
	char terminalArray[9];
	int index = 0;
	float inputFloat;
	
	while (terminalBuffer != 0x0D){ //while enter hasn't been pressed
		if (index > sizeof terminalArray){ //if end of input array
			printToTerminal("Input too long (that's what she said). Try again. \n\r");
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
		}
		else{
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
	
void generateUI(){
	
	while (1){
		printToTerminal("\n\r################ MAIN MENU #################\n\r");
		printToTerminal("1) Display all possible effects\n\r");
		printToTerminal("2) Display all added effects\n\r");
		printToTerminal("3) Remove effects\n\r");
		printToTerminal("4) Add effects\n\r");
		printToTerminal("5) Exit \n\r");
		
		waitForTerminal();
		switch (terminalBuffer){
			case '1':
				printToTerminal("1 - Echo\n\r2 - Reverb\n\r3 - Linear Gain\n\r"
					"4 - Envelope Follower\n\r TBC \n\r");
				break;
			case '2':
				printToTerminal("DISPLAY ALL EFFECTS ADDED FUNCTION\n\r");
				//printQueue();
				break;
			case '3':
				printToTerminal("Enter number of Effect to remove: \n\r");
				while (flag){
					waitForTerminal();
					switch(terminalBuffer){
						case '1':
							//dequeue(createFilterS(&echoF, 0.8));
							break;
						case '2':
							//dequeue(createFilterS(&reverbF, 0.4));
							break;
						case '3':
							//dequeue(createFilterS(&linearGainF, 1.2));
							break;
						case '4':
							//dequeue(createFilterS(&void ADC_IRQHandler(void) {envFollowerF, 0));
							break;	
						default:
							printToTerminal("Enter a correct effect number\n\r");
							flag = 1;	
					}
				break;
				}
			case '4':
				printToTerminal("Enter number of Effect Required: \n\r");
				printToTerminal("1 - Echo\n\r2 - Reverb\n\r3 - Linear Gain\n\r"
					"4 - Envelope Follower\n\r5 - Flange \n\r");	
				while (flag){
					waitForTerminal();
					switch(terminalBuffer){
						case '1':
							printToTerminal("Enter amount of Echo (e.g 0.8), then press enter: \n\r");
							waitForTerminal();
							filterVariable = getFloat(); 
							printfToTerminal("\n\r %f \n\r", filterVariable);
							enqueue(createEchoF(filterVariable, 8000));
							flag = 0;
							break;
						case '2':
							printToTerminal("Enter amount of Reverb (e.g 0.8), then press enter: \n\r");
							waitForTerminal();
							filterVariable = getFloat(); 
							//enqueue(createFilterS(&reverbF, 0.4));
							flag = 0;
							break;
						case '3':
							printToTerminal("Enter amount of Linear Gain (e.g 1.2), then press enter: \n\r");
							waitForTerminal();
							filterVariable = getFloat(); 
							//enqueue(createFilterS(&linearGainF, 1.2));
							flag = 0;
							break;
						case '4':
							printToTerminal("Enter amount of Envolope Follower (e.g 0.8), then press enter: \n\r");
							waitForTerminal();
							filterVariable = getFloat(); 
							//enqueue(createFilterS(&envFollowerF, 0));
							flag = 0;
							break;
						case '5':
							printToTerminal("Enter a mixing ratio (eg. 0.4), then press enter:\n\r");
							waitForTerminal();
							filterVariable = getFloat();
							enqueue(createFlangeF(filterVariable, 8000, 1));
							flag = 0;
							break;
						default:
							printToTerminal("Enter a correct effect number\n\r");
							flag = 1;			
					}
				}
				break;
				
			case '5':
				printToTerminal("System will now terminate");
				exit(0);
				break;
			default:
				printToTerminal("Enter a correct command\n\r");
		}
		flag = 1; //reset flag to loop through switch statements again
	}
}


