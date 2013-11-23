#ifndef UTIL_H_
#define UTIL_H_

#include "../Global.h"

void setBit(unsigned char* reg, const unsigned char bit);
void unsetBit(unsigned char* reg, const unsigned char bit);
void resetBits(unsigned char* reg);
int bitIsSet(unsigned char* reg, const unsigned char bit);

#endif /* UTIL_H_ */
