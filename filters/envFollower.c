#include "lpc_types.h"

#include <math.h>

#include "../filter.h"
#include "envFollower.h"
#include "../debug.h"
#include "../global.h"

#define ATTACK_COEFF filter->scratch[0]
#define RELEASE_COEFF filter->scratch[1]

static int32_t envelope = AMPLITUDE_PIVOT;

// See and reference:
// http://www.kvraudio.com/forum/viewtopic.php?p=5178628
//http://www.musicdsp.org/archive.php?classid=2
uint32_t envFollowerF(uint32_t sample, SFilter *filter) {

	if (sample > envelope) {
		envelope = ATTACK_COEFF * ((float) envelope -  (float) sample) + sample;
	} else {
		envelope = RELEASE_COEFF * ((float) envelope - (float) sample) + sample;
	}

	return (envelope);
}

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
