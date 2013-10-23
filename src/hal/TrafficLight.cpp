#include <iostream>
#include "../HWaccess.h"
#include "../Global.h"
#include "Util.h"
#include "TrafficLight.h"
#include "hw.h"

/// This class gives access to the Trafficlight
/// can light it green a/o yellow a/o red

static Mutex* mutex; /// the mutex for controlling the access
static TrafficLight* light; /// the Trafficlight object itself

/// TrafficLight-constructor
TrafficLight::TrafficLight() {
	mutex = new Mutex();
}

/// TrafficLight-deconstructor
TrafficLight::~TrafficLight() {
	delete mutex;
	mutex = NULL;

	delete light;
	light = NULL;
}

/// returns the only running instance of TrafficLight
TrafficLight* TrafficLight::getInstance() {
	mutex->lock();

	if (!init_HW_Done()) {
		init_HW();
	}

	if (!light) {
		light = new TrafficLight();
	}

	mutex->unlock();

	return light;
}

/// checks if the "red" bit has been set or not
int TrafficLight::statusRed() {
	return bitIsSet((unsigned char*) DIO_A, TRAFFIC_LIGHT_RED);
}

/// checks if the "yellow bit" has been set or not
int TrafficLight::statusYellow() {
	return bitIsSet((unsigned char*) DIO_A, TRAFFIC_LIGHT_YELLOW);
}

/// checks if the "green" bit has been set or not
int TrafficLight::statusGreen() {
	return bitIsSet((unsigned char*) DIO_A, TRAFFIC_LIGHT_GREEN);
}

/// sets the "red" bit
void TrafficLight::redOn() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, TRAFFIC_LIGHT_RED);
	out8(DIO_A, reg);

	mutex->unlock();
}

/// resets the "red" bit
void TrafficLight::redOff() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	unsetBit(&reg, TRAFFIC_LIGHT_RED);
	out8(DIO_A, reg);

	mutex->unlock();
}

/// sets the "yellow" bit
void TrafficLight::yellowOn() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, TRAFFIC_LIGHT_YELLOW);
	out8(DIO_A, reg);

	mutex->unlock();
}

/// resets the "yellow" bit
void TrafficLight::yellowOff() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	unsetBit(&reg, TRAFFIC_LIGHT_YELLOW);
	out8(DIO_A, reg);

	mutex->unlock();
}

/// sets the "green" bit
void TrafficLight::greenOn() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, TRAFFIC_LIGHT_GREEN);
	out8(DIO_A, reg);

	mutex->unlock();
}

/// resets the "green" bit
void TrafficLight::greenOff() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	unsetBit(&reg, TRAFFIC_LIGHT_GREEN);
	out8(DIO_A, reg);

	mutex->unlock();
}
