#include "lpc_types.h"

#include <math.h>

#include "debug.h"
#include "envFollower.h"
#include "global.h"
#include "filter.h"

#define WINDOW_SIZE 30

#define ATTACK_MS 5
#define RELEASE_MS 5

#define ATTACK_MULT 1.05
#define RELEASE_MULT 0.95

uint32_t envWindow[WINDOW_SIZE];
uint8_t oldestElem = 0;

uint32_t previousVal = 1000;

float attackCoef;
float releaseCoef;

// See and reference:
// http://www.kvraudio.com/forum/viewtopic.php?p=5178628
uint32_t envFollowerF(uint32_t sample, float nullVar, float nullVar2) {

	//envWindow[oldestElem] = sample;

	if (oldestElem == (WINDOW_SIZE - 1)) {
		oldestElem = 0;
	} else {
		oldestElem++;
	}

	uint32_t max = 0;
	
	int i;
	for (i = 0; i < WINDOW_SIZE; i++) {

		if (envWindow[i] > max) {
			max = envWindow[i];
		}
	}

	// Implements attack
	if (sample > max) {
		//max = max * ATTACK_MULT;
		max = attackCoef * (max - sample) + sample;
	} else { // Implements release
		//max = max * RELEASE_MULT;
		max = releaseCoef * (max - sample) + sample;
	}

	envWindow[oldestElem] = sample;

//	printfToTerminal("Intial: %d, final: %d\n\r", sample, max);

	return max;
}

// Initalises the necessary coefficients for implementing attack and
// release
void envFollowerInit(void) {
	
	attackCoef = exp(log(0.01) / (ATTACK_MS * ADC_SAMPLE_RATE * 0.001));
	releaseCoef = exp(log(0.01) / (RELEASE_MS * ADC_SAMPLE_RATE * 0.001));
}
