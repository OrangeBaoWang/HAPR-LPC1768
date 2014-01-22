// Created by Oliver Lea 18/01/2014

#ifndef __FILTER_H__
#define __FILTER_H__

// MUST BE INCLUDED ABOVE FILTERCHAIN.H"


// Structure containing all the information needed for a filter
// filterFunction points to the address of the destination filter
// function

typedef struct SFilter {
	uint32_t (*filterFunction)(uint32_t, float);
	float parameter;
} SFilter;

typedef struct PFilter {
	SFilter *filterOne;
	SFilter *filterTwo;
	float mixRatio;
} PFilter;

typedef struct Filter {
	PFilter *pfilter;
	SFilter *sfilter;
	int parallel;
} Filter;


uint32_t linearGainF(uint32_t sample, float gainMultiplier);

uint32_t delayF(uint32_t sample, float nullVar);

uint32_t echoF(uint32_t sample, float nullVar);

uint32_t reverbF(uint32_t sample, float nullVar);

uint32_t mixParallel(PFilter *pfilter, uint32_t sample);

SFilter *newSfilter(uint32_t (*filterAddr)(uint32_t, float),
					float filterParam);

Filter *createFilterS(uint32_t (*filterAddr)(uint32_t, float),
					float filterParam);

Filter *createFilterP(SFilter *sfilter1, SFilter *sfilter2,
						float mixingRatio);

#endif
