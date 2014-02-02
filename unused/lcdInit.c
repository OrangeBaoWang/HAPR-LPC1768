#include "lpc17xx_i2c.h"
#include "lpc17xx_pinsel.h"
#include "debug.h"
#include "i2cInit.h"
#include "string.h"
#include "interInit.h"

#define I2CDEV LPC_I2C1
#define I2C_BATRON_LCD 0x3B			//LCD is on I2C address 0x3B

#define ASCII_SHIFT 0x80
#define SEND_CHARACTER_INSR 0x40
#define FIRST_LINE_RAM_ADDR 0x80		//Highest bit one, followed by 7 bit DRAM address
#define SECOND_LINE_RAM_ADDR (0x80|0x40)	//First line starts at 0x00, second at 0x40


void lcd_transmit(void) {
	transferCfg.rx_data = NULL;
	transferCfg.rx_length = 0;
	transferCfg.retransmissions_max = 2;
	transferCfg.sl_addr7bit = I2C_BATRON_LCD;
}

void checkBusyFlag(void) {
	uint8_t *busyFlagBuf = 0;

	transferCfg.rx_data = busyFlagBuf;
	transferCfg.rx_length = 1;
	transferCfg.tx_data = NULL;
	transferCfg.tx_length = 0;

	I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_POLLING);

	while (busyFlagBuf[0] & 0x80) {
		printToTerminal("Busy flag set - WAITING\n\r");
		I2C_MasterTransferData(I2CDEV, &transferCfg, 
				I2C_TRANSFER_POLLING);
	}

	transferCfg.rx_data = NULL;
	transferCfg.rx_length = 0;
}

char asciiShift(char c) {
	
	char shiftedASCII;
	shiftedASCII = c + ASCII_SHIFT;
	
	return shiftedASCII;
}

void selectLine(int line) {

	uint8_t selectLineInstr[2];

	lcd_transmit();
	checkBusyFlag();

	transferCfg.tx_length = 2;
	transferCfg.tx_data = selectLineInstr;

	selectLineInstr[0] = 0x00; // Control byte for Instruction

	if (line == 1) {
		printToTerminal("Selecting line 1\n\r");
		selectLineInstr[1] = FIRST_LINE_RAM_ADDR;
	} else {
		printToTerminal("Selecting Line 2\n\r");
		selectLineInstr[1] = SECOND_LINE_RAM_ADDR;
	}

	I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_POLLING);
	Delay(1);
}

void sendASCII(char *string) {

	uint8_t charBuf[17];

	lcd_transmit();
	checkBusyFlag();

	transferCfg.tx_length = 17;
	transferCfg.tx_data = charBuf;

	int x;
	for (x = 1; x < 17; x++) {
		charBuf[x] = asciiShift(' ');
	}

	charBuf[0] = SEND_CHARACTER_INSR;

	for (x= 1; x < 17; x++) {
		if (*string == '\0') {
			break;
		}
		charBuf[x] = asciiShift(*string);
		string++;
	}
	I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_POLLING);
	printToTerminal("String sent\n\r");
	Delay(1);
}

void clearLine(int line) {
	
	lcd_transmit();	

	selectLine(line);
	sendASCII("");
	selectLine(line);
}

void sendChar(char character) {

	uint8_t charBuf[2];

	lcd_transmit();
	checkBusyFlag();

	transferCfg.tx_length = 2;
	transferCfg.tx_data = charBuf;

	charBuf[0] = SEND_CHARACTER_INSR;

	charBuf[1] = asciiShift(character);

	I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_POLLING);
	printToTerminal("Char sent\n\r");
	Delay(1);
}

void lcd_init() { //	https://bitbucket.org/furyfire/raspi/src/6c8ca895cf0071496037ef376df4b6f44170cca3/hw/pcf2119x/main.c?at=default
	
        uint8_t buf[13];

	printToTerminal("Initilizing LCD...");

	checkBusyFlag();

        buf[0] = 0x00; // Control byte for Instruction
	
        buf[1] = 0x34; // DL: 8 bits, M:  16 by two line  display, SL: 1:18, H: normal instruction set
        buf[2] = 0x0C; // D: Display on, C: curser off, B: blink off

        buf[3] = 0x06; // I/D: increment, S: no shift
        buf[4] = 0x35; // DL: 8 bits, M: 16 by two line, SL: 1:18, H: extended instruction set
        buf[5] = 0x04; // P: left to right, Q: top to bottom
        buf[6] = 0x10; // TC1: 0, TC2: 0

        buf[7] = 0x42; // HV Stages 3
        buf[8] = 0x9f; // set Vlcd, store to VA
        buf[9] = 0x34; // DL: 8 bits, M:  two line, SL: 1:18, H: normal instruction set
        buf[10] = 0x80; // DDRAM Address set to 00hex

        buf[11] = 0x02; // return home

	buf[12] = 0x01; // clear entire display and sets DRAM address 0 in address counter


	transferCfg.rx_data = NULL;
	transferCfg.rx_length = 0;
	transferCfg.retransmissions_max = 2;
	transferCfg.sl_addr7bit = I2C_BATRON_LCD;

	transferCfg.tx_data = buf;
	transferCfg.tx_length = 13;

	I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_POLLING);

	clearLine(1);
	clearLine(2);
	selectLine(1);

	printToTerminal("COMPLETE\n\r");
}
