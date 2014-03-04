// Created by Oliver Lea - 24/2/2014

// O-Edit: Fixed errors - 24/2/2014
// O-Edit: Added reading wdt at the end of the sampling rate - 25/2/2014
// O-Edit: WDT, found out counter always starts at 255 - 26/2/2014
// O-Edit: WDT finished and working - 4/3/2014

#include "lpc17xx_wdt.h"

#include "global.h"
#include "debug.h"

void WDT_IRQHandler(void) {

	NVIC_DisableIRQ(WDT_IRQn);

	clearScreen();

	printToTerminal("\n\rWATCHDOG TIMEOUT\n\r");
	printToTerminal("The effect generator will now reset...\n\r");
	printToTerminal("\n\rPress any key to continue");

	waitForTerminal();

	NVIC_SystemReset();
}

void watchdog_init(void) {

	// Initialize the timer. Counting by the internal clock and interrupting
	// when the maximum value of the timer is reached
	WDT_Init(WDT_CLKSRC_IRC, WDT_MODE_INT_ONLY);

	// Enable the watchdog timer interrupt
	NVIC_EnableIRQ(WDT_IRQn);

	// Sets the watchdog timer to generate an interrupt after 255uS plus the
	// time interval between each sampling interrupt (approx 22uS);
	// WDT always contains an intial 0xFF in the counter register
	// If a number below 0xFF is tried to be used with WDT_Start() then the
	// value in the counter register defaults to 0xFF.
	// The counter register is decremented until it reaches zero. When this happens
	// the interrupt is generated
	WDT_Start(WDT_TIMEOUT_US);
}
