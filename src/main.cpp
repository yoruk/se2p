#include <cstdlib>
#include <iostream>
#include "milestone1/Milestone1.h"
#include "tests/Test_Mutex.h"
#include "tests/Test_Util.h"
#include "tests/Test_Gate.h"
#include "tests/Test_TrafficLight.h"
#include "tests/Test_Conveyor.h"
#include "tests/Test_Led.h"
#include "HWaccess.h"
#include "milestone2/Simple_Milestone2.h"

// zum debuggen
#include "unistd.h"
#include "Global.h"
#include "hal/Util.h"
#include "hal/hw.h"
#include "hal/Gate.h"


int main(int argc, char *argv[]) {
#ifdef SIMULATION
	printf("Simulation is running\n");fflush(stdout);
	IOaccess_open(); // Open connection to simulation
#endif

	//return milestone1_start();
	//return test_Mutex_start();
	//test_Util_start();

	//test_Gate_start();
	//test_Led_start();
	//test_TrafficLight_start();
	//test_Conveyor_start();

	simple_milestone2_start();

	// DEBUG!!!

//	printf("real-life test!\n");fflush(stdout);
//	Gate* gate = Gate::getInstance();
//
//	init_HW();
//
//	gate->open();
//	printf("bit set\n");fflush(stdout);
//
//	sleep(2);
//
//	gate->close();
//	printf("bit unset\n");fflush(stdout);

#ifdef SIMULATION
	IOaccess_close(); // Open connection to simulation
	printf("Simulation is running\n");fflush(stdout);
#endif
	return EXIT_SUCCESS;
}
