#ifndef __OVERDRIVE_H_
#define __OVERDRIVE_H_

uint32_t overDriveF(uint32_t sample, SFilter *filter);

void printOverDriveF(SFilter *filter);

Filter *createOverDriveF(float gain, float drive);

#endif
