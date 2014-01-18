#include "lpc17xx_i2c.h"
#include "lpc17xx_pinsel.h"
#include "debug.h"

#define I2CDEV LPC_I2C1

PINSEL_CFG_Type PinCfg;
I2C_M_SETUP_Type transferCfg;

void pollAddresses(void) {
	//debug_init(); must be initialized first in main function
	uint8_t devicesCon = 0;

	while (transferCfg.sl_addr7bit <= 127) {
		if (I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_POLLING)
			== ERROR) {
			transferCfg.sl_addr7bit++;
		} else {
			devicesCon++;
			printfToTerminal("Address %d is used\n\r",
				transferCfg.sl_addr7bit);
			transferCfg.sl_addr7bit++;
		}
	}
}

int i2c_init(void) {

	PINSEL_CFG_Type PinCfg;

	PinCfg.Pinmode = 0;

	// I2C pin connect
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	
	PinCfg.Funcnum = 3;
	PinCfg.Pinnum = 0;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 1;
	PINSEL_ConfigPin(&PinCfg);

	//Init slave i2c peripheral
	I2C_Init(I2CDEV, 100000);
	
	//Enable i2c operation
	I2C_Cmd(I2CDEV, ENABLE);

	return 0;
}
