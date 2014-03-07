// Created by Oliver Lea - 13/01/2014

// O-Edit: Added median input filter - 27/01/2014
// D-Edit: Added ADC pin setup for infrared sensor - 25/02/2014

#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"

#include "adcInit.h"

// Calculates the median from the three ADC inputs from the
// analogue source, and then returns the median
uint16_t getAdcSample(void) {

	uint16_t sample4 = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_4);
	uint16_t sample0 = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_0);
	uint16_t sample1 = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_1);

	uint16_t medianSamp;

	if (sample4 > sample1) {
		if (sample4 < sample0) {
			medianSamp = sample4;
		} else if (sample1 > sample0) {
			medianSamp  = sample1;
		} else {
			medianSamp = sample0;
		}
	} else {
		if (sample1 < sample0) {
			medianSamp = sample1;
		} else if (sample4 > sample0) {
			medianSamp = sample4;
		} else {
			medianSamp = sample0;
		}
	}

	return medianSamp;
}

void sadc_init(int sampleRate) {

	PINSEL_CFG_Type PinCfg;

	// Pin used for sound input
	PinCfg.Funcnum = 3;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 30;		// Refers to pin 19 on board
	PinCfg.Portnum = 1;
	PINSEL_ConfigPin(&PinCfg);

	// Pin used for sound input
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 23;		// Refers to pin 15 on board
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);

	// Pin used for sound input
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 24;		// Refers to pin 16 on board
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	
	// Pin used for the infrared sensor
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 25;		// Refers to pin 17 on board
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);

	ADC_Init(LPC_ADC, sampleRate);

	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_4, ENABLE);
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE);
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_1, ENABLE);

	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_2, ENABLE); //infrared channel

	ADC_BurstCmd(LPC_ADC, ENABLE);

	return;
}

//Add into main file void ADC_IRQHandler(void) with the IRQ handler
