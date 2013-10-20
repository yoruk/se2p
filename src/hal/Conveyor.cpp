#include <iostream>
#include "../HWaccess.h"
#include "../Global.h"
#include "Util.h"
#include "Conveyor.h"
#include "hw.h"

static Mutex* mutex = new Mutex();
static Conveyor* conveyor;

Conveyor::Conveyor() {
}

Conveyor::~Conveyor() {
}

Conveyor* Conveyor::getInstance() {
	if (!init_HW_Done()) {
		init_HW();
	}

	if (!conveyor) {
		conveyor = new Conveyor();
	}

	return conveyor;
}

int Conveyor::getDirection() {
	if (bitIsSet((unsigned char*) DIO_A, CONVEYOR_RIGHT)) {
		return CONVEYOR_DIRECTION_RIGHT;
	} else {
		return CONVEYOR_DIRECTION_LEFT;
	}
}

int Conveyor::getSpeed() {
	if (bitIsSet((unsigned char*) DIO_A, CONVEYOR_STOP)) {
		return CONVEYOR_STOPPED;
	} else if (bitIsSet((unsigned char*) DIO_A, CONVEYOR_SLOW)) {
		return CONVEYOR_RUNNING_SLOW;
	} else {
		return CONVEYOR_RUNNING_FAST;
	}
}

void Conveyor::moveRight() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	unsetBit(&reg, CONVEYOR_LEFT);
	setBit(&reg, CONVEYOR_RIGHT);
	out8(DIO_A, reg);

	mutex->unlock();
}

void Conveyor::moveLeft() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	unsetBit(&reg, CONVEYOR_RIGHT);
	setBit(&reg, CONVEYOR_LEFT);
	out8(DIO_A, reg);

	mutex->unlock();
}

void Conveyor::moveSlow() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, CONVEYOR_SLOW);
	out8(DIO_A, reg);

	mutex->unlock();
}

void Conveyor::conveyorStop() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, CONVEYOR_STOP);
	out8(DIO_A, reg);

	mutex->unlock();
}

void Conveyor::conveyorContinue() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	unsetBit(&reg, CONVEYOR_STOP);
	out8(DIO_A, reg);

	mutex->unlock();
}
