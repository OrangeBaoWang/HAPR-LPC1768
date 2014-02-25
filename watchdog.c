
#include "lpc17xx_wdt.h"

#include "global.h"
#include "debug.h"

void WDT_IRQHandler(void) {

	printToTerminal("\n\rWATCHDOG TIMEOUT\n\r");

	// Clear the timeout flag
	WDT_ClrTimeOutFlag();
}

void watchdog_init(void) {

	// Initialize the timer. Counting by the internal clock and interrupting
	// when the maximum value of the timer is reached
	WDT_Init(WDT_CLKSRC_IRC, WDT_MODE_INT_ONLY);

	// Enable the watchdog timer interrupt
	NVIC_EnableIRQ(WDT_IRQn);

	// Sets the watchdog timer to generate an interrupt after twice the
	// number of microseconds as the rate of the sampling
	WDT_Start(WDT_TIMEOUT_US - 40);

	printfToTerminal("Timeout is %d\n\r", WDT_TIMEOUT_US);
}
