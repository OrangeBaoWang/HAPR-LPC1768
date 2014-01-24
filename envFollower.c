
#include "lpc_types.h"

#include <math.h>

#include "debug.h"
#include "envFollower.h"
#include "global.h"
#include "filter.h"

#define WINDOW_SIZE 20

#define SMOOTHING_MULT 0.8
#define SMOOTHING_EXP 1.01

float follower = 0;
uint32_t envWindow[WINDOW_SIZE];
uint8_t oldestElem = 0;

// See and reference:
// http://www.kvraudio.com/forum/viewtopic.php?p=5178628
uint32_t updateFollower(uint32_t sample, float nullVar) {

	envWindow[oldestElem] = sample;

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

	if (max > sample) {
		//max  = pow((max * SMOOTHING_MULT), SMOOTHING_EXP);
		max = max * SMOOTHING_MULT;
	}
	//printfToTerminal("Max: %d", max);
	return max;
}
