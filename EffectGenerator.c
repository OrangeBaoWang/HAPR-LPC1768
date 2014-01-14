#include "lpc17xx_adc.h"
#include "string.h"

#include "debug.h"
//#include "i2cInit.h"
#include "lcdInit.h"
#include "adcInit.h"

char toPrint[50];
long int sampleCounter = 0;

const float voltageScalar = 3.3 / 4096.0;

void ADC_IRQHandler(void) {

	float analogueValue = voltageScalar * (float)getAdcData();

	//sprintf(toPrint, "Value is %0.4f\n\r", (float)((analogueValue >> 4)/100));
	//printToTerminal(toPrint);
	sprintf(toPrint, "Distance is %000.3f cm\n\r", 21.67 / (analogueValue - 0.15));
	//sprintf(toPrint, "%000.3f cm\n\r", analogueValue);
	printToTerminal(toPrint);
}

int main(void) {

	//i2c_init();
	debug_init();
	//inter_init();
	//lcd_init();
	//sadc_init(500);

	ASSERT(0, "Zero is not one");

	while(1) {
	}
}
