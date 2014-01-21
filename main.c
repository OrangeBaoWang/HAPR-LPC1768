#include "lpc17xx_adc.h"
#include "lpc17xx_dac.h"
#include "lpc_types.h"

#include "stdlib.h"

#include "debug.h"
#include "adcInit.h"
#include "dacInit.h"
#include "filter.h"
#include "filterChain.h"

#define ADC_SAMPLE_RATE	44000
#define BUFFER_SIZE 8192

uint16_t sampleBuffer[BUFFER_SIZE];
uint16_t *sampleP = sampleBuffer;
uint16_t output;
uint16_t *medianVal;

char recieveBuffer[10];

Filter *testFilter;
Filter *linearFilter;

// Interrupt handler that samples the ADC and sends the sample
// on to be filtered
void ADC_IRQHandler(void) {

	*sampleP = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_4);

/*
	if (sampleP < &(sampleBuffer[2])) {
		output = applyFilters(*sampleP);
	} else {
			//finds median sample value from previous 3
			if (*sampleP > *(sampleP - 1)) {
				if (*sampleP < *(sampleP - 2)) {
					*medianVal= *sampleP;
				}
				else if (*(sampleP - 1) > *(sampleP - 2)) {
					*medianVal = *(sampleP - 1);
				}
				else *medianVal = *(sampleP - 2);
			}
			else {
				if(*(sampleP - 1) < *(sampleP - 2)){
					*medianVal = *(sampleP - 1);	
				}
				else if (*sampleP > *(sampleP - 2)){
					*medianVal = *sampleP;
				}
				else *medianVal = *(sampleP - 2);
			}
			output = applyFilters(*medianVal);
	}
*/

	output = applyFilters(*sampleP);

	dacSetValue(output>>2);

	//Select next buffer location based on previous location
	//When at the end of the buffer, loop round to the beginning next
	if (sampleP < &(sampleBuffer[BUFFER_SIZE - 1])) {
		sampleP++;
	} else {
		sampleP = sampleBuffer;
	}
}

uint32_t testFilterFunction(uint32_t testSample, uint32_t param1) {

	return testSample;
}

int main(void) {

	debug_init();

	chain_init();

	enqueue(createFilterS(&linear
GainF, 2));

	/*
	enqueue(createFilterP((newSfilter(&linearGainF, 5)),
				newSfilter(&linearGainF, 10), 0.7));

	uint16_t test = applyFilters(10);

	printfToTerminal("Test is: %d\n\r", test);
	*/
/*
	testFilter = malloc(sizeof(Filter));
	linearFilter = malloc(sizeof(Filter));

	testFilter->filterFunction = &testFilterFunction;
	testFilter->parameter = 42;
	enqueue(testFilter);
	
	linearFilter->filterFunction = &linearGain;
	linearFilter->parameter = 5;
	enqueue(linearFilter);
	enqueue(linearFilter);
	dequeue(linearFilter);
	dequeue(testFilter);
	dequeue(linearFilter);

	uint16_t test = applyFilters(102);

	printfToTerminal("Test = %i\n\r", test);
	dequeue(testFilter);
*/

	sadc_init(ADC_SAMPLE_RATE);
	sdac_init();

	while(1) {
		//read_usb_serial_none_blocking(&recieveBuffer, 10);
		//printfToTerminal("stuff %c \n\r", recieveBuffer);
		//some awesome stuff that'll read and print the greatest things
		//ever.
		
		
	}

	return 0;
}
