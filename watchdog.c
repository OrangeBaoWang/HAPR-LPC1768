
#include "lpc17xx_wdt.h"

#include "global.h"
#include "debug.h"

void WDT_IRQHandler(void) {

	printToTerminal("\n\rWATCHDOG TIMEOUT\n\r");

	// Clear the timeout flag
	WDT_ClrTimeOutFlag();

	WDT_Feed();
}

void watchdog_init(void) {

	// Initialize the timer. Counting by the internal clock and interrupting
	// when the maximum value of the timer is reached
	WDT_Init(WDT_CLKSRC_IRC, WDT_MODE_INT_ONLY);

	// Enable the watchdog timer interrupt
	NVIC_EnableIRQ(WDT_IRQn);

	// Sets the watchdog timer to generate an interrupt after twice the
	// number of microseconds as the rate of the sampling
	// WDT always contains an intial 0xFF in the counter register
	// If a number below 0xFF is tried to be used with WDT_Start() then the
	// value in the counter register defaults to 0xFF.
	// The counter register is decremented until it reaches zero. When this happens
	// the interrupt is generated
	WDT_Start(WDT_TIMEOUT_US + 255);

	WDT_Feed();

	/*

	wdtCounter = WDT_GetCurrentCount();

	printfToTerminal("Timeout is %d\n\rTime is:%d\n\r", WDT_TIMEOUT_US, wdtCounter - 200);

		wdtCounter = WDT_GetCurrentCount();

	printfToTerminal("Timeout is %d\n\rTime is:%d\n\r", WDT_TIMEOUT_US, wdtCounter);
		wdtCounter = WDT_GetCurrentCount();

	printfToTerminal("Timeout is %d\n\rTime is:%d\n\r", WDT_TIMEOUT_US, wdtCounter);

		wdtCounter = WDT_GetCurrentCount();

	printfToTerminal("Timeout is %d\n\rTime is:%d\n\r", WDT_TIMEOUT_US, wdtCounter);

	*/

	//NVIC_DisableIRQ(WDT_IRQn);
}
