#include "lpc17xx_adc.h"
#include "lpc17xx_dac.h"

#include "debug.h"
#include "adcInit.h"
#include "dacInit.h"

#define ADC_SAMPLE_RATE	44000
#define BUFFER_SIZE 4096

uint16_t sampleBuffer[BUFFER_SIZE];
uint16_t *sampleP = sampleBuffer;

void ADC_IRQHandler(void) {

	*sampleP = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_4);

	if (sampleP < &(sampleBuffer[2])) {
		dacSetValue(*sampleP >> 2);
	//printfToTerminal("%d\n\r", *sampleP);
	} else {
		//if (*sampleP > *(sampleP - 1)) {
			//dacSetValue(*(sampleP - 1));
		//}
		//else {
			dacSetValue(((*sampleP + *(sampleP - 1) + *(sampleP - 2)) / 3));
		//}
	}

	//Select next buffer location based on previous location
	//When at the end of the buffer, loop round to the beginning next
	//sampleP = sampleBuffer + ((uint16_t)++sampleP % BUFFER_SIZE);


	if (sampleP < &(sampleBuffer[BUFFER_SIZE - 1])) {
		sampleP++;
	} else {
		sampleP = sampleBuffer;
	}
}

// Assuming the given value fits criteria, function 
// called and gain applied
uint16_t upwardCompression(uint16_t analogueValue) {

	
}

int main(void) {

	debug_init();

	sadc_init(ADC_SAMPLE_RATE);
	sdac_init();

	while(1) {
	}
}
