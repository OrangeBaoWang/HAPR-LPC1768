#include "lpc_types.h"
#include "lpc17xx_uart.h"

#include <stdlib.h>
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

uint32_t received;

//recieves single char from keyboard input
uint32_t receiveFromTerminal() {
	return UART_Receive((LPC_UART_TypeDef *) LPC_UART0, &terminalBuffer, 1, NONE_BLOCKING);
}

//waits for keyboard input
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
	while (terminalBuffer != 13){
		terminalArray[index] = terminalBuffer;
		printfToTerminal("%c", terminalBuffer);
		index++; 
		waitForTerminal();
	}
	terminalArray[index] = '\0'; //add null char at end so it recognises string
	index = 0;
	printfToTerminal("\n\r %s \n\r", terminalArray);
	filterVariable = atof(terminalArray); //convert char array to float
	printfToTerminal("%f \n\r", filterVariable);
}
		
void generateUI(){
	
	while (1){
		printToTerminal("################ MAIN MENU #################\n\r");
		printToTerminal("1) Display all possible effects\n\r");
		printToTerminal("2) Display all added effects\n\r");
		printToTerminal("3) Remove effects\n\r");
		printToTerminal("4) Add effects\n\r");
		printToTerminal("5) Exit\n\r");
		
		received = receiveFromTerminal();
		while (received > 0){
			received = 0;
			switch (terminalBuffer){
				case '1':
					printToTerminal("1 - Echo\n\r 2 - Reverb\n\r 3 - Linear Gain\n\r"
						"4 - Envelope Follower\n\r TBC \n\r" );
					break;
				case '2':
					printToTerminal("DISPLAY ALL EFFECTS ADDED FUNCTION");
					//printQueue();
					break;
				case '3':
					printToTerminal("Enter number of Effect to remove: ");
					while (!received){
						received = receiveFromTerminal();
					}
					received = 0;
					switch(terminalBuffer){
						case '1':
							dequeue(createFilterS(&echoF, 0.8));
							break;
						case '2':
							dequeue(createFilterS(&reverbF, 0.4));
							break;
						case '3':
							dequeue(createFilterS(&linearGainF, 1.2));
							break;
						case '4':
							dequeue(createFilterS(&void ADC_IRQHandler(void) {envFollowerF, 0));
							break;		
					}
					break;
				case '4':
					printToTerminal("Enter number of Effect Required: \n\r");
					printToTerminal("1 - Echo\n\r 2 - Reverb\n\r 3 - Linear Gain\n\r"
								"4 - Envelope Follower\n\r TBC \n\r");
					waitForTerminal();
					switch(terminalBuffer){
						case '1':
							printToTerminal("Enter amount of Echo (e.g 0.8), then press enter: \n\r");
							
							waitForTerminal();

							getFloat() = filterVariable;
							
							printfToTerminal("%f \n\r", filterVariable);
							
							enqueue(createFilterS(&echoF, filterVariable));
							break;
						case '2':
							enqueue(createFilterS(&reverbF, 0.4));
							break;
						case '3':
							enqueue(createFilterS(&linearGainF, 1.2));
							break;
						case '4':
							enqueue(createFilterS(&envFollowerF, 0));
							break;		
					}
					break;
					
				case '5':
					printToTerminal("System will now terminate");
					exit(0);
					break;
				default:
					printToTerminal("Enter a correct command dick");
			}
		}
	}
}

