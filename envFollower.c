
#include "lpc_types.h"

#include "debug.h"
#include "envFollower.h"
#include "global.h"
#include "filter.h"

#define WINDOW_SIZE 30

#define RELEASE_MULT 0.8
#define ATTACK_MULT 1.05

uint32_t envWindow[WINDOW_SIZE];
uint8_t oldestElem = 0;

uint32_t previousMax = 0;

// See and reference:
// http://www.kvraudio.com/forum/viewtopic.php?p=5178628
uint32_t envFollowerF(uint32_t sample, float nullVar) {

	envWindow[oldestElem] = sample;

	if (oldestElem == (WINDOW_SIZE - 1)) {
		oldestElem = 0;
	} else {
		oldestElem++;
	}

	uint32_t max = 0;

	// Implements attack
	//if (sample > previousMax) {
		//max = previousMax * ATTACK_MULT;
		//previousMax = sample;
	//} else { // Implements release
		int i;
		for (i = 0; i < WINDOW_SIZE; i++) {

			if (envWindow[i] > max) {
				max = envWindow[i];
			}
		}

		if (max > sample) {
			max = max * RELEASE_MULT;
		}
	//}

	//printfToTerminal("Max: %d", max);
	return max;
}
