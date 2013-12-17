#include "TrafficLightGelbBlinken.h"

static Mutex* mutex = new Mutex();

TrafficLightGelbBlinken::TrafficLightGelbBlinken() {
	// TODO Auto-generated constructor stub
}

TrafficLightGelbBlinken::~TrafficLightGelbBlinken() {
	// TODO Auto-generated destructor stub
}

void TrafficLightGelbBlinken::shutdown() {
	printf("TRAFICLIGHT BLINKEN ENDING");fflush(stdout);
}

void TrafficLightGelbBlinken::execute(void *arg) {
	mutex->lock();
	printf("TRAFICLIGHT BLINKEN RUNING");fflush(stdout);
	unsigned char reg = in8(DIO_A);
	while (!isStopped()) {
		setBit(&reg, TRAFFIC_LIGHT_YELLOW);
		out8(DIO_A, reg);
		usleep(160000);
		unsetBit(&reg, TRAFFIC_LIGHT_YELLOW);
		out8(DIO_A, reg);
		usleep(160000);
	}
	mutex->unlock();
	printf("TRAFICLIGHT BLINKEN RUNING");fflush(stdout);
}
