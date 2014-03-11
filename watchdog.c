// Created by Oliver Lea - 24/2/2014

// THIS FILE IS PART OF OLIVER LEA'S INDIVIDUAL SECTION

// O-Edit: Fixed errors - 24/2/2014

// O-Edit: Added reading wdt at the end of the sampling - 25/2/2014

// O-Edit: Added a work in progress printUsage() function that should tell the user
//	how hard the CPU is working, and the current sample rate - 25/02/2014

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

void printUsage(void) {

	// usedTime is the percentage of the available time for each interrupt
	// that is being used
	int32_t usedTime;

	uint32_t difference;

	uint32_t sRate = ADC_SAMPLE_RATE;

	// Remaining used to calculate how much whitespace needs to be printed
	// for when the usedTime is < 100%
	uint32_t remaining = 0;

	difference = WDT_TIMEOUT_US - wdtCounter;

	// Calculates the percentage of the time being used for each sampling ISR
	// compared to how long between interrupts at SAMPLE_RATE_US
	usedTime = (((float) difference / (float) SAMPLE_RATE_US) * 100);

	// If the usedTime percentage is greater than 100%, calculate an approximation
	// for the current actual sampling rate
	if (usedTime > 100) {
		sRate = (ADC_SAMPLE_RATE / ((float) usedTime / 100.0));
	}

	printfToTerminal("Approx. Sample Rate:\t%dHz\n\r", sRate);

	printfToTerminal("CPU Usage:\n\r\t\t%d%%\n\r\t\t|", usedTime);

	if (usedTime > 100) {
		printToTerminal("==========|");

		while (usedTime > 100) {
			usedTime -= 10;
			printToTerminal("=");
		}
		printToTerminal(">\n\r\n\r");

	} else {
		while (usedTime > 0) {
			remaining++;
			usedTime -= 10;
			printToTerminal("=");
		}
		printToTerminal(">");

		// Calculate the number of spaces that need to be printed
		remaining = 10 - remaining;

		int i;
		for (i = 0; i < remaining; i++) {
			printToTerminal(" ");
		}
		printToTerminal("|\n\r\n\r");
	}

	return;
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
