#include <iostream>
#include "../HWaccess.h"
#include "../Global.h"
#include "Util.h"
#include "TrafficLight.h"
#include "hw.h"

static Mutex* mutex = new Mutex();
static TrafficLight* light;

TrafficLight::TrafficLight() {}

TrafficLight::~TrafficLight() {}

TrafficLight* TrafficLight::getInstance() {
	if (!init_HW_Done()) {
		init_HW();
	}

	if (!light) {
		light = new TrafficLight();
	}

	return light;
}

int TrafficLight::statusRed() {
	return bitIsSet((unsigned char*) DIO_A, TRAFFIC_LIGHT_RED);
}

int TrafficLight::statusYellow() {
	return bitIsSet((unsigned char*) DIO_A, TRAFFIC_LIGHT_YELLOW);
}

int TrafficLight::statusGreen() {
	return bitIsSet((unsigned char*) DIO_A, TRAFFIC_LIGHT_GREEN);
}

void TrafficLight::redOn() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, TRAFFIC_LIGHT_RED);
	out8(DIO_A, reg);

	mutex->unlock();
}

void TrafficLight::redOff() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	unsetBit(&reg, TRAFFIC_LIGHT_RED);
	out8(DIO_A, reg);

	mutex->unlock();
}

void TrafficLight::yellowOn() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, TRAFFIC_LIGHT_YELLOW);
	out8(DIO_A, reg);

	mutex->unlock();
}

void TrafficLight::yellowOff() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	unsetBit(&reg, TRAFFIC_LIGHT_YELLOW);
	out8(DIO_A, reg);

	mutex->unlock();
}

void TrafficLight::greenOn() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, TRAFFIC_LIGHT_GREEN);
	out8(DIO_A, reg);

	mutex->unlock();
}

void TrafficLight::greenOff() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	unsetBit(&reg, TRAFFIC_LIGHT_GREEN);
	out8(DIO_A, reg);

	mutex->unlock();
}
