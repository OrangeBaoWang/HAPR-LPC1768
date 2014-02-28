#include "lpc17xx_adc.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_wdt.h"
#include "lpc17xx_timer.h"
#include "lpc_types.h"

#include <stdlib.h>

#include "global.h"
#include "debug.h"
#include "adcInit.h"
#include "timerInit.h"
#include "watchdog.h"
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
#include "filters/overDrive.h"
#include "filters/lowPassNoise.h"

uint16_t sampleBuffer[BUFFER_SIZE];
uint16_t *sampleP = sampleBuffer;

// Variable to hold the counter value of the WDT at the end of the sampling ISR
volatile uint32_t wdtCounter = 0;

// 0 if pass-through not selected. 1 if pass-through is selected
volatile uint8_t passThrough = 0;

// 0 if infraMix not selected. 1 if infraMix selected
volatile uint8_t infraMix = 0;

float infraValue; //infrared sensor value
//float scalar = 3.3 / 4096.0;

uint16_t output;

// Interrupt handler that samples the ADC and sends the sample
// on to be filtered
void TIMER0_IRQHandler(void) {

	WDT_Feed();

	*sampleP = getAdcSample();

	if (passThrough) {
		output = *sampleP;
	} else if (infraMix) {

	  // infraValue will be between 0 (no hand) and 1 (hand close)
	  infraValue = (ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_2)) / 4000;
	  output = (*sampleP * (1 - infraValue)) + (applyFilters(*sampleP) * infraValue);

	  printfToTerminal("infrared: %f\n\r", infraValue);
	  printfToTerminal("ADC_CHANNEL_2: %d\n\r", ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_2));

	} else {
		output = applyFilters(*sampleP);
	}

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

	wdtCounter = WDT_GetCurrentCount();

	return;
}

void tests() {

	enqueue(createReverbF(0.4, 7000));

	enqueue(createEchoF(0.8, 8000));
	enqueue(createEchoF(0.8, 8000));

	enqueue(createDelayF(8000));

	enqueue(createReverbF(0.4, 7000));

	enqueue(createEchoF(0.8, 8000));
	enqueue(createEchoF(0.8, 8000));

	enqueue(createDelayF(8000));

	enqueue(createEnvFollowerF(5, 5));

	enqueue(createTremeloF(0.5,2));

	enqueue(createFlangeF(0.5, 8000, 1));

	enqueue(createLinearGainF(1.05));

	enqueue(createReverbF(0.4, 7000));

	enqueue(createEchoF(0.8, 8000));
	enqueue(createEchoF(0.8, 8000));

	enqueue(createDelayF(8000));

	enqueue(createReverbF(0.4, 7000));

	enqueue(createEchoF(0.8, 8000));
	enqueue(createEchoF(0.8, 8000));



/*
	enqueue(createFilterP((createFlangeF(0.4, 8000, 0.5)),
				createReverbF(0.4, 8000), 0.5));
*/
	return;
}

int main(void) {

	debug_init();
	printToTerminal("UART INITIALISED\n\r");

	chain_init();
	printfToTerminal("FILTER CHAIN INITIALISED. SAMPLE RATE IS :%d\n\r", ADC_SAMPLE_RATE);

	sadc_init(ADC_SAMPLE_RATE);
	sdac_init();
	sample_timer_init(SAMPLE_RATE_US);
	watchdog_init();


	tests();

	generateUI();

	return 0;
}
