#include "TrafficLightBlinken.h"

TrafficLightBlinken* TrafficLightBlinken::instance = NULL;
static Mutex* mutex = new Mutex();

TrafficLightBlinken::TrafficLightBlinken() {
}

TrafficLightBlinken::~TrafficLightBlinken() {
	delete instance;
	instance = NULL;
}

TrafficLightBlinken* TrafficLightBlinken::getInstance() {
	mutex->lock();
	if (!init_HW_Done()) {
		init_HW();
	}

	if (!instance) {
		instance = new TrafficLightBlinken();
	}
	mutex->unlock();

	return instance;
}

void TrafficLightBlinken::shutdown() {
}

void TrafficLightBlinken::execute(void *arg) {
	mutex->lock();
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
}
