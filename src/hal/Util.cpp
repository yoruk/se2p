#include <stdlib.h>
#include "Util.h"

void setBit(unsigned char* reg, const unsigned bit) {
	*reg |= bit;
}

void unsetBit(unsigned char* reg, const unsigned bit) {
	*reg &= ~bit;
}
