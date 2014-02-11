#include "lpc17xx_adc.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_timer.h"
#include "lpc_types.h"

#include <stdlib.h>

#include "global.h"
#include "debug.h"
#include "adcInit.h"
#include "timerInit.h"
#include "dacInit.h"
#include "filter.h"
#include "filterChain.h"
#include "userInterface.h"

#include "filters/linearGain.h"
#include "filters/flange.h"
#include "filters/delay.h"
#include "filters/reverb.h"
#include "filters/envFollower.h"
#include "filters/echo.h"
#include "filters/tremelo.h"

uint16_t sampleBuffer[BUFFER_SIZE];
uint16_t *sampleP = sampleBuffer;

uint16_t output;

char recieveBuffer[10];


// Interrupt handler that samples the ADC and sends the sample
// on to be filtered
void TIMER0_IRQHandler(void) {

	*sampleP = getAdcSample();
	output = applyFilters(*sampleP);

	dacSetValue(output>>2);

	//Select next buffer location based on previous location
	//When at the end of the buffer, loop round to the beginning next
	if (sampleP < &(sampleBuffer[BUFFER_SIZE - 1])) {
		sampleP++;
	} else {
		sampleP = sampleBuffer;
	}

	// Reset the timer (it stops when it has counted to the designated value)
	// and begin counting again
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);

	return;
}

void tests() {

//	enqueue(createReverbF(0.4, 7000));

//	enqueue(createEchoF(0.8, 8000));
//	enqueue(createEchoF(0.8, 8000));

//	enqueue(createDelayF(8000));

//	enqueue(createEnvFollowerF(5, 5));

//	enqueue(createTremeloF(0.5,2));

	enqueue(createFlangeF(0.5, 8000, 1));
	dequeue(createFlangeF(0.5, 8000, 1));

//	enqueue(createLinearGainF(1.05));


	enqueue(createFilterP((createFlangeF(0.4, 8000, 10)),
				createReverbF(0.4, 8000), 0.5));
/*
	dequeue(createFilterP((createFlangeF(0.4, 8000, 10)),
				createReverbF(0.4, 8000), 0.5));
*/
}

int main(void) {

	debug_init();
	printToTerminal("UART INITIALISED\n\r");

	chain_init();
	printfToTerminal("FILTER CHAIN INITIALISED. SAMPLE RATE IS :%d\n\r", ADC_SAMPLE_RATE);

	tests();

	printQueue();

	sadc_init(ADC_SAMPLE_RATE);
	sdac_init();
	timer_init((uint32_t) ((1.0/ADC_SAMPLE_RATE) * 1000000));

	while(1) {
		//read_usb_serial_none_blocking(&recieveBuffer, 10);
		//printfToTerminal("stuff %c \n\r", recieveBuffer);
		//some awesome stuff that'll read and print the greatest things
		//ever.
		generateUI();
		
	}

	return 0;
}
