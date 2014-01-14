#include "lpc17xx_adc.h"
#include "string.h"
#include "lpc17xx_dac.h"

#include "debug.h"
//#include "i2cInit.h"
#include "lcdInit.h"
#include "adcInit.h"
#include "dacInit.h"

char toPrint[50];
long int sampleCounter = 0;

const float voltageScalar = 3.3 / 4096.0;

void ADC_IRQHandler(void) {

	uint16_t analogueValue = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_4);

	//sprintf(toPrint, "Value is %0.4f\n\r", (float)((analogueValue >> 4)/100));
	//printToTerminal(toPrint);
	//sprintf(toPrint, "Distance is %000.3f cm\n\r", 21.67 / (analogueValue - 0.15));
	//sprintf(toPrint, "%000.3f cm\n\r", analogueValue);
	//printToTerminal(toPrint);

	//printfToTerminal("Value read is %f\n\r", (float)analogueValue * voltageScalar);

	dacSetValue(analogueValue >> 2);
}

int main(void) {

	//i2c_init();
	debug_init();
	//inter_init();
	//lcd_init();
	sadc_init(10000);
	sdac_init();

	//ASSERT(0, "Zero is not one");

	while(1) {
	}
}
