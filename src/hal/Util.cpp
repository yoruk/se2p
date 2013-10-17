#include <stdlib.h>
#include <stdbool.h>
#include "Util.h"

void setBit(unsigned char* reg, const unsigned bit) {
	*reg |= bit;
}

void unsetBit(unsigned char* reg, const unsigned bit) {
	*reg &= ~bit;
}

bool bitIsSet(unsigned char* reg, const unsigned bit) {
	if((*reg & bit) != 0) {
		return true;
	} else {
		return false;
	}
}
