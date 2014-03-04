#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "debug.h"

uint8_t terminalBuffer;

// Flag to check if there has been a keyboard input
static uint32_t received;

//Read
int read_usb_serial_none_blocking(char *buf, int length) {

	return (UART_Receive((LPC_UART_TypeDef *) LPC_UART0, (uint8_t *) buf, length,
		NONE_BLOCKING));
}

//Write
int write_usb_serial_blocking(char *buf, int length) {

	return (UART_Send((LPC_UART_TypeDef *) LPC_UART0, (uint8_t *) buf, 
		length, BLOCKING));
}

// Recieves single char from keyboard input
uint32_t receiveFromTerminal(void) {
	return UART_Receive((LPC_UART_TypeDef *) LPC_UART0, &terminalBuffer, 1, NONE_BLOCKING);
}

// Waits for keyboard input. loops until user inputs something
void waitForTerminal(void) {

	while (!received) {
		received = receiveFromTerminal();
	}
	received = 0;
}

int printToTerminal(char *buf) {

	return (UART_Send((LPC_UART_TypeDef *) LPC_UART0, (uint8_t *) buf, 
		(strlen(buf) + 1), BLOCKING));	
}

int printfToTerminal(char *buf, ...) {

	char charBuf[BUF_MAX];

	va_list vl;
	va_start(vl, buf);		//Find the stack pointer for the arg list

	vsnprintf(charBuf, BUF_MAX, buf, vl);
	va_end(vl);

	return (UART_Send((LPC_UART_TypeDef *) LPC_UART0, (uint8_t *) charBuf, 
		(strlen(charBuf) + 1), BLOCKING));
}

// Clears the screen using an ASCII escape character and sets the cursor to
// the top left-hand corner of the screen
void clearScreen(void) {
	
	printfToTerminal("\033[2J");
	printfToTerminal("\033[H");
}

void setColor(char *color) {

	if (!strcmp(color, "red")) {
		printfToTerminal(KRED "");
	} else if (!strcmp(color, "green")) {
		printfToTerminal(KGRN "");
	} else if (!strcmp(color, "yellow")) {
		printfToTerminal(KYEL "");
	} else if (!strcmp(color, "blue")) {
		printfToTerminal(KBLU "");
	} else if (!strcmp(color, "magenta")) {
		printfToTerminal(KMAG "");
	} else if (!strcmp(color, "cyan")) {
		printfToTerminal(KCYN "");
	} else if (!strcmp(color, "white")) {
		printfToTerminal(KWHT "");
	} else {
		printfToTerminal(KNRM "");
	}
}

void debugAssert(char *message, char *file, int line) {
	
	setColor("red");
	printfToTerminal("ASSERTION ERROR in %s line %d\n\r%s\n\r", file, line, message);
	while(1);
}

void debugWarn(char *message, char *file, int line) {

	setColor("red");
	printfToTerminal("WARNING in %s line %d\n\r%s\n\r", file, line, message);
	setColor("default");
}

void debugThrow(char *message, char *file, int line) {

	setColor("red");
	printfToTerminal("ERROR THROWN in %s line %d\n\r%s\n\r", file, line, message);
	while(1);
}

//Init code for the UART USB line
void debug_init(void) {

	UART_CFG_Type UARTConfigStruct;			//UART configuration structure
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;	//UART FIFO configuration struct
	PINSEL_CFG_Type PinCfg;				//Pin configuration for UART

	//UART PIN SETTINGS
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	//USB Serial port settings - Tx
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 2;
	PINSEL_ConfigPin(&PinCfg);
	//USB Serial port settings - Rx
	PinCfg.Pinnum = 3;
	PINSEL_ConfigPin(&PinCfg);

	/* Initialize UART Configuration parameter structure to default state:
	 * - Baudrate = 9600bps
	 * - 8 data bit
	 * - 1 Stop bit
	 * - None parity
	 */
	UART_ConfigStructInit(&UARTConfigStruct);

	/* Initialize FIFOConfigStruct to default state:
	 * - FIFO_DMAMode = DISABLE
	 * - FIFO_Level = UART_FIFO_TRGLEV0
	 * - FIFO_ResetRxBuf = ENABLE
	 * - FIFO_ResetTxBuf = ENABLE
	 * - FIFO_State = ENABLE
	 */
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

	//Initialize UART0 peripheral with given to corresponding parameter
	UART_Init((LPC_UART_TypeDef *) LPC_UART0, &UARTConfigStruct);

	//Initialize FIFO for UART0 peripheral
	UART_FIFOConfig((LPC_UART_TypeDef *) LPC_UART0, &UARTFIFOConfigStruct);

	//Enable UART Transmit
	UART_TxCmd((LPC_UART_TypeDef *) LPC_UART0, ENABLE);

	clearScreen();

	printToTerminal("__CONSOLE_BEGIN__\n\r\n\r");
}
