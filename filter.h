// Created by Oliver Lea 18/01/2014

#ifndef __FILTER_H__
#define __FILTER_H__

// MUST BE INCLUDED ABOVE FILTERCHAIN.H"


// Structure containing all the information needed for a filter
// filterFunction points to the address of the destination filter
// function

typedef struct SFilter {
	uint32_t (*filterFunction)(uint32_t, struct SFilter *);
	void (*printFunction)(struct SFilter *);
	float parameters[5];
	float scratch[5];
} SFilter;

typedef struct PFilter {
	SFilter *filterOne;
	SFilter *filterTwo;
	float mixRatio;
} PFilter;

typedef struct Filter {
	PFilter *pfilter;
	SFilter *sfilter;
} Filter;

uint32_t mixParallel(PFilter *pfilter, uint32_t sample);

/*
SFilter *newSfilter(uint32_t (*filterAddr)(uint32_t, SFilter *), void (*printAddr)(SFilter *),
		float param0, float param1, float param2, float param3, float param4);
*/
Filter *createFilterS(uint32_t (*filterAddr)(uint32_t, SFilter *), void (*printAddr)(SFilter *),
		float param0, float param1, float param2, float param3, float param4);

Filter *createFilterP(Filter *filter1, Filter *filter2,
						float mixingRatio);

#endif
