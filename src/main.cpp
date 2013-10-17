#include <cstdlib>
#include <iostream>
#include "milestone1/Milestone1.h"
#include "tests/Test_Mutex.h"
#include "tests/Test_Util.h"
#include "tests/Test_Gate.h"
#include "HWaccess.h"

int main(int argc, char *argv[]) {
#ifdef SIMULATION
	printf("Simulation is running\n");fflush(stdout);
	IOaccess_open(); // Open connection to simulation
#endif

	//return milestone1_start();
	//return test_Mutex_start();

	//test_Util_start();
	test_Gate_start();

#ifdef SIMULATION
	IOaccess_close(); // Open connection to simulation
#endif

	return EXIT_SUCCESS;
}
