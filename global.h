#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#define ADC_SAMPLE_RATE	44000
#define BUFFER_SIZE 8192

// For passing parameters to the createFilterS function that requires
// five floats, even if some of them are unused by the filteer
#define UNUSED 0

extern uint16_t sampleBuffer[BUFFER_SIZE];
extern uint16_t *sampleP;




#endif
