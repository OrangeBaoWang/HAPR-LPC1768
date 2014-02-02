#ifndef __ENVFOLLOWER_H__
#define __ENVFOLLOWER_H__

uint32_t envFollowerF(uint32_t sample, float parameters[5]);

void printEnvFollowerF(float parameters[5]);

Filter *createEnvFollowerF(float attack_ms, float release_ms);

#endif
