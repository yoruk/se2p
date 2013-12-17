#include "TrafficLightBlinken.h"

TrafficLightBlinken* TrafficLightBlinken::instance = NULL;
static Mutex* mutex = new Mutex();

TrafficLightBlinken::TrafficLightBlinken() {
}

TrafficLightBlinken::~TrafficLightBlinken() {
}


void TrafficLightBlinken::shutdown() {
	printf("TRAFICLIGHT BLINKEN ENDING");fflush(stdout);
}

void TrafficLightBlinken::execute(void *arg) {
	mutex->lock();
	printf("TRAFICLIGHT BLINKEN RUNING");fflush(stdout);
	unsigned char reg = in8(DIO_A);
	while (!isStopped()) {
		setBit(&reg, TRAFFIC_LIGHT_RED);
		out8(DIO_A, reg);
		usleep(160000);
		unsetBit(&reg, TRAFFIC_LIGHT_RED);
		out8(DIO_A, reg);
		usleep(160000);
	}
	mutex->unlock();
	printf("TRAFICLIGHT BLINKEN RUNING");fflush(stdout);
}
