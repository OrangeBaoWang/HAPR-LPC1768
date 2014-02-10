#include "lpc17xx_timer.h"

TIM_TIMERCFG_Type timer_cfg;
TIM_MATCHCFG_Type timerMatch_cfg;

void timerReset(void) {

	TIM_ResetCounter(LPC_TIM0);
}

void timerClearInterrupt(void) {

	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
}

void disableTimer(void) {
	TIM_Cmd(LPC_TIM0, DISABLE);
}

void enableTimer(void) {
	TIM_Cmd(LPC_TIM0, ENABLE);
}

void timer_init(uint32_t timeInterval) {
	
	//Use microsecond time intervals
	timer_cfg.PrescaleOption = TIM_PRESCALE_USVAL;
	timer_cfg.PrescaleValue = 1;
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timer_cfg);

	timerMatch_cfg.MatchChannel = 0;
	timerMatch_cfg.IntOnMatch   = TRUE;
	timerMatch_cfg.ResetOnMatch = TRUE;
	timerMatch_cfg.StopOnMatch  = FALSE;
	timerMatch_cfg.ExtMatchOutputType = TIM_EXTMATCH_TOGGLE;
	timerMatch_cfg.MatchValue   = timeInterval;
	TIM_ConfigMatch(LPC_TIM0,&timerMatch_cfg);
	
   	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(TIMER0_IRQn, ((0x01<<3)|0x01));
	/* Enable interrupt for timer 0 */
	NVIC_EnableIRQ(TIMER0_IRQn);
	// To start timer 0
	TIM_Cmd(LPC_TIM0,ENABLE);
}
