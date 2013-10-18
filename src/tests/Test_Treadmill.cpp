#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../hal/Treadmill.h"

#define RUNTIME 10

void test_Treadmill_start(){

	Treadmill* treadmill = Treadmill::getInstance();
	int i;

	for(i=0; i<RUNTIME; i++) {

		treadmill->moveRight();
		sleep(1);
		treadmill->moveSlow();
		sleep(1);
		treadmill->moveLeft();
		sleep(1);
		treadmill->treadmilStop();
		sleep(1);
		}

	}

