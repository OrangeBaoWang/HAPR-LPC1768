#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "debug.h"

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

void dumpRegs() { //http://www.ethernut.de/en/documents/arm-inline-asm.html

	setColor("cyan");
        printfToTerminal("DEBUG: Dumping registers:\n\r");
        int tmp;
        __asm volatile("mov %[res], r0\n\t" : [res] "=r" (tmp));
        printfToTerminal("r0: 0x%x\n\r", tmp);
        __asm volatile("mov %[res], r1\n\t" : [res] "=r" (tmp));
        printfToTerminal("r1: 0x%x\n\r", tmp);
        __asm volatile("mov %[res], r2\n\t" : [res] "=r" (tmp));
        printfToTerminal("r2: 0x%x\n\r", tmp);
        __asm volatile("mov %[res], r3\n\t" : [res] "=r" (tmp));
        printfToTerminal("r3: 0x%x\n\r", tmp);
        __asm volatile("mov %[res], r4\n\t" : [res] "=r" (tmp));
        printfToTerminal("r4: 0x%x\n\r", tmp);
        __asm volatile("mov %[res], r5\n\t" : [res] "=r" (tmp));
        printfToTerminal("r5: 0x%x\n\r", tmp);
        __asm volatile("mov %[res], r6\n\t" : [res] "=r" (tmp));
        printfToTerminal("r6: 0x%x\n\r", tmp);
        __asm volatile("mov %[res], r7\n\t" : [res] "=r" (tmp));
        printfToTerminal("r7: 0x%x\n\r", tmp);
        __asm volatile("mov %[res], r8\n\t" : [res] "=r" (tmp));
        printfToTerminal("r8: 0x%x\n\r", tmp);
        __asm volatile("mov %[res], r9\n\t" : [res] "=r" (tmp));
        printfToTerminal("r9: 0x%x\n\r", tmp);
        __asm volatile("mov %[res], r10\n\t" : [res] "=r" (tmp));
        printfToTerminal("r10: 0x%x\n\r", tmp);
        __asm volatile("mov %[res], r11\n\t" : [res] "=r" (tmp));
        printfToTerminal("r11: 0x%x\n\r", tmp);
        __asm volatile("mov %[res], r12\n\t" : [res] "=r" (tmp));
        printfToTerminal("r12: 0x%x\n\r", tmp);
        __asm volatile("mov %[res], r13\n\t" : [res] "=r" (tmp));
        printfToTerminal("r13: 0x%x\n\r", tmp);
        __asm volatile("mov %[res], r14\n\t" : [res] "=r" (tmp));
        printfToTerminal("r14: 0x%x\n\r", tmp);
        __asm volatile("mov %[res], r15\n\t" : [res] "=r" (tmp));
        printfToTerminal("r15: 0x%x\n\r", tmp);
        printfToTerminal("---------------------------------------------\n\r");
        __asm volatile("mov %[res], sp\n\t" : [res] "=r" (tmp));
        printfToTerminal("sp: 0x%x\n\r", tmp);
        __asm volatile("mov %[res], fp\n\t" : [res] "=r" (tmp));
        printfToTerminal("fp: 0x%x\n\r", tmp);
        __asm volatile("mov %[res], lr\n\t" : [res] "=r" (tmp));
        printfToTerminal("lr: 0x%x\n\r", tmp);

	setColor("default");
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
	dumpRegs();
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
	dumpRegs();
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

	printToTerminal("__CONSOLE_BEGIN__\n\r\n\r");
}
