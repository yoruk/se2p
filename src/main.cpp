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
#include "tests/Test_Simple_Serial.h"
#include "tests/Test_SerialCom.h"
#include "tests/Test_Sensorik.h"
#include "tests/Test_Dispatcher.h"
#include "tests/Test_Timer.h"
#include "tests/Test_Puk.h"
#include "tests/Test_Fifo.h"

#include "examples/msgSendReceiveReply.h"
#include "examples/msgSendReceivePulse.h"
#include "examples/msgSendReceivePulse_2x.h"
#include "examples/msgDeliverEvent.h"

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
	//test_Simple_Serial_start();
	test_SerialCom_start();
	//test_Sensorik();
	//test_Dispatcher();
	//test_Timer_start();
	//test_Puk_start();
	//test_Fifo_start();

	//msgSendReceiveReply_start();
	//msgSendReceivePulse_start();
	//msgSendReceivePulse_2x_start();
	//msgDeliverEvent_start();
	//simple_milestone2_start();

	return EXIT_SUCCESS;
}
