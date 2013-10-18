#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../hal/Conveyor.h"

#define RUNTIME 10

void test_Conveyor_start(){

	Conveyor* conveyor = Conveyor::getInstance();
	int i;

	for(i=0; i<RUNTIME; i++) {

		conveyor->moveRight();
		sleep(1);
		conveyor->moveSlow();
		sleep(1);
		conveyor->moveLeft();
		sleep(1);
		conveyor->conveyorStop();
		sleep(1);
		}

	}

