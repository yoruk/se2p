#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../hal/Gate.h"

#define RUNTIME 10

void test_Gate_start() {
	Gate* gate = Gate::getInstance();
	int i;

	for(i=0; i<RUNTIME; i++) {
		sleep(1);
		gate->open();
		sleep(1);
		gate->close();
	}
}
