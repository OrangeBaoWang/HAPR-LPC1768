// Created by Oliver Lea - 31/01/2014

#ifndef __ENVFOLLOWER_H__
#define __ENVFOLLOWER_H__

uint32_t envFollowerF(uint32_t sample, SFilter *filter);

void printEnvFollowerF(SFilter *filter);

Filter *createEnvFollowerF(float attack_ms, float release_ms);

#endif
