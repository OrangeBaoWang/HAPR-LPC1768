#ifndef __TIMERINIT_H__
#define __TIMERINIT_H__

void timerReset(void);

void timerClearInterrupt(void);

void disableTimer(void);

void enableTimer(void);

void timer_init(uint32_t timeInterval);

#endif
