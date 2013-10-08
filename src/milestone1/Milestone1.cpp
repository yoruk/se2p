#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include "MyThread.h"
#include "Global.h"
#include "HWaccess.h"

using namespace thread;

#define RUNTIME 10

int milestone1_start() {
#ifdef SIMULATION
	cout << "Simulation is running" << endl;
	IOaccess_open(); // Open connection to simulation
#endif

	std::cout << "milestone1_start() is running" << std::endl;

	// get access rights for hardware
	if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
		perror("milestone1_start(): ThreadCtl access failed\n");
		return -1;
	}

	// Initialize digital io card
	out8(DIGITAL_CARD_CONTROL, 0x8A);

	MyThread thread;
	thread.start(NULL);

	sleep(RUNTIME);

	thread.stop();
	thread.join();

#ifdef SIMULATION
	IOaccess_close(); // Open connection to simulation
#endif

	return EXIT_SUCCESS;
}
