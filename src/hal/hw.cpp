#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include "../HWaccess.h"
#include "hw.h"
#include "../Global.h"

static bool done = false;

void init_HW() {
	if(!done) {
#ifdef SIMULATION
	printf("Simulation is running\n");fflush(stdout);
	IOaccess_open(); // Open connection to simulation
#endif

	// get access rights for hardware
	if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
		perror("milestone1_start(): ThreadCtl access failed\n");
		exit(EXIT_FAILURE);
	}

	// Initialize digital io card
	out8(DIGITAL_CARD_CONTROL, 0x8A);

#ifdef SIMULATION
	IOaccess_close(); // Open connection to simulation
#endif

	done = true;
	}
}

bool init_HW_Done() {
	return done;
}
