#include "lpc17xx_i2c.h"
#include "lpc17xx_pinsel.h"
#include "debug.h"
#include "i2cInit.h"
#include "lcdInit.h"
#include "interInit.h"

#define KEYPAD	0x21
#define I2CDEV LPC_I2C1

#define COLUMN1	0x7F	//112
#define COLUMN2	0xBF	//176
#define COLUMN3	0xDF	//208
#define COLUMN4	0xEF	//224

#define ROW1	0xF7
#define ROW2	0xFB
#define ROW3	0xFD
#define ROW4	0xFE

uint8_t txBuffer;
uint8_t rxBuffer;
char charMatrix[4][4] = {{'*', '0', '#', 'D'}, {'7', '8', '9', 'C'}, {'4', '5', '6', 'B'}, {'1', '2', '3', 'A'}};
char *toPrint[50];

void keypad_transmit(void) {

	transferCfg.retransmissions_max = 2;
	transferCfg.sl_addr7bit = KEYPAD;

	transferCfg.rx_length = 0;
	transferCfg.rx_data = NULL;

	transferCfg.tx_length = 1;
	transferCfg.tx_data = &txBuffer;

}

void keypad_receive(void) {
	
	transferCfg.retransmissions_max = 2;
	transferCfg.sl_addr7bit = KEYPAD;

	transferCfg.rx_length = 1;
	transferCfg.rx_data = &rxBuffer;

	transferCfg.tx_length = 0;
	transferCfg.tx_data = NULL;

}

void transmitAndReceive(void) {

	keypad_transmit();

	I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_POLLING);

	Delay(1);

	keypad_receive();
	I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_POLLING);
}

int buttonPressed(void) {
	if ((rxBuffer & 0x0F) != 0x0F) {	//Button pressed
		return 1;
	} else {
		return 0;
	}
}

short int determineRow(void) {

	uint32_t receivedRow = ~(rxBuffer & 0x0F);

	short int row;
	for (row = 0; row < 4; row++) {
		if (1<<row & receivedRow) {
			return row;
		}
	}
}

char scanKeypad(void) {
	int columnNo = 0;
	int rowNo = 0;

	switch(columnNo) {
		case(0):
			txBuffer = COLUMN1;
			transmitAndReceive();
			if (buttonPressed() == 1) {
				rowNo = determineRow();
				return charMatrix[rowNo][columnNo];
			}
			columnNo++;
			rowNo++;
		case(1):
			txBuffer = COLUMN2;
			transmitAndReceive();

			if (buttonPressed() == 1) {
				rowNo = determineRow();
				return charMatrix[rowNo][columnNo];
			}
			columnNo++;
			rowNo++;
		case(2):
			txBuffer = COLUMN3;
			transmitAndReceive();

			if (buttonPressed() == 1) {
				rowNo = determineRow();
				return charMatrix[rowNo][columnNo];
			}
			columnNo++;
			rowNo++;
		case(3):
			txBuffer = COLUMN4;
			transmitAndReceive();

			if (buttonPressed() == 1) {
				rowNo = determineRow();
				return charMatrix[rowNo][columnNo];
			}
	}

	return 'X';		//If no button pressed	
}

char forceGetKeypadValue() {

	char keyPressed = 'X';

	while (keyPressed == 'X') {
		keyPressed = scanKeypad();
	}
	return keyPressed;
}
