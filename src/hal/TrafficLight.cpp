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
	return bitIsSet((unsigned char*) DIO_A, RED);
}

int TrafficLight::statusYellow() {
	return bitIsSet((unsigned char*) DIO_A, YELLOW);
}

int TrafficLight::statusGreen() {
	return bitIsSet((unsigned char*) DIO_A, GREEN);
}

void TrafficLight::redOn() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, RED);
	out8(DIO_A, reg);

	mutex->unlock();
}

void TrafficLight::redOff() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	unsetBit(&reg, RED);
	out8(DIO_A, reg);

	mutex->unlock();
}

void TrafficLight::yellowOn() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, YELLOW);
	out8(DIO_A, reg);

	mutex->unlock();
}

void TrafficLight::yellowOff() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	unsetBit(&reg, YELLOW);
	out8(DIO_A, reg);

	mutex->unlock();
}

void TrafficLight::greenOn() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, GREEN);
	out8(DIO_A, reg);

	mutex->unlock();
}

void TrafficLight::greenOff() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	unsetBit(&reg, GREEN);
	out8(DIO_A, reg);

	mutex->unlock();
}
