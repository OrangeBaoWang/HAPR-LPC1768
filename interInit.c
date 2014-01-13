#include "lpc17xx_systick.h"		// Interrupt timer

volatile unsigned long tickCounter;

void SysTick_Handler(void) {
	tickCounter++;				//Execute ISR
}

void Delay(unsigned long tick) {

	unsigned long tempTickCounter = tickCounter;
	
	while ((tickCounter - tempTickCounter) < tick);
}

void inter_init(void) {
	
	tickCounter = 0;

	SYSTICK_InternalInit(10);		//Set interrupt every 10ms

	SYSTICK_IntCmd(ENABLE);			//Enable interrupts to take place

	SYSTICK_Cmd(ENABLE);			//Begin counting
}
