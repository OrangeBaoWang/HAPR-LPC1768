#include "lpc17xx_adc.h"
#include "lpc17xx_dac.h"
#include "stdlib.h"

#include "debug.h"
#include "adcInit.h"
#include "dacInit.h"
#include "filter.h"
#include "filterChain.h"

#define ADC_SAMPLE_RATE	44000
#define BUFFER_SIZE 4096

uint16_t sampleBuffer[BUFFER_SIZE];
uint16_t *sampleP = sampleBuffer;
uint16_t medianSample = 0;

void ADC_IRQHandler(void) {

	*sampleP = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_4);

	if (sampleP < &(sampleBuffer[2])) {
		dacSetValue(*sampleP >> 2);
	} else {
		//if (*sampleP > *(sampleP - 1)) {
			//dacSetValue(*(sampleP - 1));
		//}
		//else {
			//finds median sample value from previous 3
			if (*sampleP > *(sampleP - 1)) {
				if (*sampleP < *(sampleP - 2)) {
					dacSetValue(*sampleP >> 2);
				}
				else if (*(sampleP - 1) > *(sampleP - 2)) {
					dacSetValue(*(sampleP - 1) >> 2);
				}
				else dacSetValue(*(sampleP - 2) >> 2);
			}
			else {
				if(*(sampleP - 1) < *(sampleP - 2)){
					dacSetValue(*(sampleP - 1) >> 2);	
				}
				else if (*sampleP > *(sampleP - 2)){
					dacSetValue(*sampleP >> 2);
				}
				else dacSetValue(*(sampleP - 2) >> 2);
			}
		//}
	}

	//Select next buffer location based on previous location
	//When at the end of the buffer, loop round to the beginning next
	if (sampleP < &(sampleBuffer[BUFFER_SIZE - 1])) {
		sampleP++;
	} else {
		sampleP = sampleBuffer;
	}
}

int testFilterFunction(int param0, int param1) {

	printfToTerminal("HERE. Param0 = %i, Param1 = %i", param0, param1);

	return 1;
}

int main(void) {

	debug_init();

	chain_init();

	Filter *testFilter;
	testFilter = malloc(sizeof(Filter));

	testFilter->filterFunction = &testFilterFunction;
	testFilter->parameters[0] = 42;
	testFilter->parameters[1] = 54;
	enqueue(testFilter);

	int test = (*(testFilter->filterFunction))(testFilter->parameters[0],
					testFilter->parameters[1]);

	printfToTerminal("Test = %i", test);

	dequeue(testFilter);

	/* COMMENTED OUT FOR TESTING
	sadc_init(ADC_SAMPLE_RATE);
	sdac_init();

	while(1) {
	}
	*/

	return 0;
}
