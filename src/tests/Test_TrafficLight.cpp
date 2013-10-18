#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../hal/TrafficLight.h"

#define RUNTIME 10

void test_TrafficLight_start(){

	TrafficLight* light = TrafficLight::getInstance();
	int i;

	for(i=0; i<RUNTIME; i++) {
		light->greenOn();
		sleep(1);
		light->greenOff();
		sleep(1);
		light->redOn();
		sleep(1);
		light->redOff();
		sleep(1);
		light->yellowOn();
		sleep(1);
		light->yellowOff();
		sleep(1);
	}

}


