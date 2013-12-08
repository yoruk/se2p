#include "Test_TrafficLight.h"

#define RUNTIME 10

void test_TrafficLight_start(){

	TrafficLight* light = TrafficLight::getInstance();
	int i;

//		for (i = 0; i < RUNTIME; i++) {
//		light->greenOn();
//		sleep(1);
//		light->greenOff();
//		sleep(1);
//		light->redOn();
//		sleep(1);
//		light->redOff();
//		sleep(1);
//		light->yellowOn();
//		sleep(1);
//		light->yellowOff();
//		sleep(1);
//	}
	light->greenOn();
	sleep(2);
	light->greenOff();
	light->greenOff();
	sleep(2);
	light->greenOn();
	sleep(2);
	light->greenOff();
	light->greenOff();
	light->yellowOn();
//	light->flashRedOn();
//	sleep(6);
//	light->flashRedOff();
//	sleep(2);

}


