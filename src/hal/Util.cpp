#include "Util.h"
#include "Global.h"

/// This module contents some util functions
/// which are used for setting/resetting bits
/// and for checking if a bit has been set.

/// sets a bit in a register
///
/// \param	reg		the register that holds the bit to be set
/// \param	bit		bitmask with the bit to be set
void setBit(unsigned char* reg, const unsigned char bit) {
	*reg |= bit;
}

/// unsets a bit in a register
///
/// \param	reg		the register that holds the bit to be unset
/// \param	bit		bitmask with the bit to be unset
void unsetBit(unsigned char* reg, const unsigned char bit) {
	*reg &= ~bit;
}

void resetBits(unsigned char* reg) {
	*reg = BIT_NONE;
}

/// checks if a certain bit is set
///
/// \param	reg		the register that holds the bit to be checked
/// \param	bit		bitmask with the bit to be checked
/// \return 		bit has been set or not
int bitIsSet(unsigned char* reg, const unsigned char bit) {
	if((*reg & bit) != 0) {
		return true;
	} else {
		return false;
	}
}
