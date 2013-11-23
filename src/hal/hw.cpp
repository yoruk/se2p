#include "hw.h"

/// This module holds the function for
/// getting the correct access right to the hardware

/// hardware has been intialized or not
static bool done = false;

/// initializes the hardware
void init_HW() {
	if(!done) {
		// get access rights for hardware
		if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
			perror("milestone1_start(): ThreadCtl access failed\n");
			exit(EXIT_FAILURE);
		}

		// Initialize digital io card
		out8(DIGITAL_CARD_CONTROL, 0x8A);

		done = true;
	}
}

/// checks if hardware has been initialized
/// \return		yes or no
bool init_HW_Done() {
	return done;
}
