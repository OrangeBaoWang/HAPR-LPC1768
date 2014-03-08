// Created by Oliver Lea - 21/01/2014

// O-Edit: Added the global declaration of ADC_SAMPLE_RATE - 22/01/2014

// O-Edit: Added globally visible sampleBuffer and sampleP for use throughout
//		the program, and defined UNUSED as 0 - 02/02/2014

// O-Edit: Defined AMPLITUDE_MAX to be 4096 (highest input from the ADC) and
//		defined AMPLITUDE_PIVOT to be AMPLITUDE_MAX/2 - 24/02/2014

// O-Edit: Defined SAMPLE_RATE_US which is the sample rate converted into the
//		time between each sample in microseconds. Also added a globally visible
//		passThrough boolean which defines the state (on/off) of pass-through - 24/02/2014

// O-Edit: Added a new global varaible (wdtCounter) which counts the time between
//		each watchdog feed and the end of the sampling ISR. Also added a new constant
//		called WDT_TIMEOUT_US which is the timeout time of the watchdog timer in
//		microseconds - 25/02/2014

// D-Edit: Added new global variable (infraMix) that is used to determine whether infrared
//		mixing is on or off - 25/02/2014

// O-Edit: Added new global variable terminalBuffer that is used to store ASCII chars
//		read from the terminal - 04/03/2014

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#define ADC_SAMPLE_RATE	44000

// Sample rate in microseconds per interrupt
#define SAMPLE_RATE_US ((uint32_t) ((1.0/ADC_SAMPLE_RATE) * 1000000))

// WDT reset value is 0xFF plus the number of microseconds passed to the
// WDT start function
#define WDT_TIMEOUT_US (SAMPLE_RATE_US + 0xFF)

#define AMPLITUDE_MAX 4096
#define AMPLITUDE_PIVOT AMPLITUDE_MAX/2
#define BUFFER_SIZE 8192

// For passing parameters to the createFilterS function that requires
// five floats, even if some of them are unused by the filteer
#define UNUSED 0

extern uint16_t sampleBuffer[BUFFER_SIZE];
extern uint16_t *sampleP;

extern volatile uint32_t wdtCounter;

extern volatile uint8_t passThrough;

extern volatile uint8_t infraMix;

// Buffer to store read values from terminal
extern uint8_t terminalBuffer;


#endif
