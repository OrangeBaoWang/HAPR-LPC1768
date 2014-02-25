#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#define ADC_SAMPLE_RATE	44000

// Sample rate in microseconds per interrupt
#define SAMPLE_RATE_US ((uint32_t) ((1.0/ADC_SAMPLE_RATE) * 1000000))
#define WDT_TIMEOUT_US (SAMPLE_RATE_US * 2)

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


#endif
