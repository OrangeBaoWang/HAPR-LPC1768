#include "lpc_types.h"

#include <math.h>

#include "../filter.h"
#include "envFollower.h"
#include "../debug.h"
#include "../global.h"

#define WINDOW_SIZE 2

#define ATTACK_COEFF filter->scratch[0]
#define RELEASE_COEFF filter->scratch[1]

//static uint32_t envWindow[WINDOW_SIZE];
//static uint8_t oldestElem;

uint32_t envelope = 0;

uint32_t envWindow[WINDOW_SIZE];
int index = 1; //used to check where in envWindow

// See and reference:
// http://www.kvraudio.com/forum/viewtopic.php?p=5178628
//http://www.musicdsp.org/archive.php?classid=2

// ONLY ONE ENVELOPE FOLLOWER CAN BE IN THE FILTER CHAIN AT ANY
// ONE TIME
uint32_t envFollowerF(uint32_t sample, SFilter *filter) {
  
  envWindow[index] = sample;

	envelope = envWindow[index - 1]; //get previous value
  //attack, follow rising wave
  if (sample > envelope) {
    envelope = ATTACK_COEFF * (envelope - sample) + sample;
  } else {
  //release, follow falling wave
    envelope = RELEASE_COEFF  * (envelope - sample) + sample;
  }
  
  //if at end of envWindow, loop back round
	if (index == (WINDOW_SIZE - 1)) {
		index = 0;
	} else {
		index++;
	}
	
  printfToTerminal("env value %d \n\r", envelope);
  //printfToTerminal("attack %f \n", ATTACK_COEFF);
  //printfToTerminal("release %f \n", RELEASE_COEFF);
  return envelope;

}
/*
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
		max = ATTACK_COEFF * (max - sample) + sample;
	} else { // Implements release
		//max = max * RELEASE_MULT;
		max = RELEASE_COEFF * (max - sample) + sample;
	}

	envWindow[oldestElem] = sample;

//	printfToTerminal("Intial: %d, final: %d\n\r", sample, max);

	return max;
*/

void printEnvFollowerF(SFilter *filter) {

	printfToTerminal("Envelope Follower:\n\r\t\tAttack (ms): %f\n\r\t\tRelease (ms): %f\n\r\r",
			filter->parameters[0], filter->parameters[1]);
}

// Initalises the necessary coefficients for implementing attack and
// release
Filter *createEnvFollowerF(float attack_ms, float release_ms) {
	
	Filter *envFollow = createFilterS(&envFollowerF, &printEnvFollowerF, attack_ms, release_ms,
			UNUSED, UNUSED, UNUSED);

	(envFollow->sfilter)->scratch[0] = exp(log(0.01) / (attack_ms * ADC_SAMPLE_RATE * 0.001));
	(envFollow->sfilter)->scratch[1] = exp(log(0.01) / (release_ms * ADC_SAMPLE_RATE * 0.001));

	return envFollow;
}
