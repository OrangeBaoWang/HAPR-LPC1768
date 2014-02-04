#include "lpc_types.h"

#include <math.h>

#include "../filter.h"
#include "envFollower.h"
#include "../debug.h"
#include "../global.h"

#define WINDOW_SIZE 30

static uint32_t envWindow[WINDOW_SIZE];
static uint8_t oldestElem = 0;

//static uint32_t previousVal = 1000;

static float attackCoef;
static float releaseCoef;

// See and reference:
// http://www.kvraudio.com/forum/viewtopic.php?p=5178628
uint32_t envFollowerF(uint32_t sample, float parameters[5]) {

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

void printEnvFollowerF(float parameters[5]) {

	printfToTerminal("Envelope Follower:\n\r\t\tAttack (ms): %f\n\r\t\tRelease (ms): %f\n\r\r",
			parameters[0], parameters[1]);
}

// Initalises the necessary coefficients for implementing attack and
// release
Filter *createEnvFollowerF(float attack_ms, float release_ms) {
	
	attackCoef = exp(log(0.01) / (attack_ms * ADC_SAMPLE_RATE * 0.001));
	releaseCoef = exp(log(0.01) / (release_ms * ADC_SAMPLE_RATE * 0.001));

	Filter *envFollow = createFilterS(&envFollowerF, &printEnvFollowerF, attack_ms, release_ms,
			UNUSED, UNUSED, UNUSED);

	return envFollow;
}
