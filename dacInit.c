//Created by Oliver Lea
//Modified by Oliver Lea 14/01/2014 - Added comments

#include "lpc17xx_dac.h"
#include "lpc17xx_pinsel.h"

void sdac_init(void) {
	PINSEL_CFG_Type PinCfg;

	PinCfg.Funcnum = 2;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 26;		//Refers to pin 18 on board
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);

	DAC_Init(LPC_DAC);
}

/*  Unnecessary function call
    Call DAC_UpdateData directly */
void dacSetValue(int newValue) {
	DAC_UpdateValue(LPC_DAC, newValue);
}
