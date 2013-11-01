#include <cstdlib>
#include <iostream>
#include "HWaccess.h"

#include "tests/Test_Mutex.h"
#include "tests/Test_Util.h"
#include "tests/Test_Gate.h"
#include "tests/Test_Simple_Gate.h"
#include "tests/Test_TrafficLight.h"
#include "tests/Test_Conveyor.h"
#include "tests/Test_Led.h"
#include "tests/Test_Serial.h"

#include "examples/msgSendReceiveReply/msgSendReceiveReply.h"

#include "milestone1/Milestone1.h"
#include "milestone2/Simple_Milestone2.h"


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
	//test_Simple_Gate_start();
	//test_Serial_start();

	msgSendReceiveReply_start();

	//simple_milestone2_start();


#ifdef SIMULATION
	IOaccess_close(); // Open connection to simulation
	printf("Simulation is running\n");fflush(stdout);
#endif
	return EXIT_SUCCESS;
}
