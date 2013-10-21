#ifndef UTIL_H_
#define UTIL_H_

void setBit(unsigned char* reg, const unsigned char bit);
void unsetBit(unsigned char* reg, const unsigned char bit);
int bitIsSet(unsigned char* reg, const unsigned char bit);

#endif /* UTIL_H_ */
