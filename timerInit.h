// Created by Oliver Lea - 13/01/2014

#ifndef __TIMERINIT_H__
#define __TIMERINIT_H__

void timerReset(void);

void timerClearInterrupt(void);

void disableTimer(void);

void enableTimer(void);

void sample_timer_init(uint32_t timeInterval);

#endif
