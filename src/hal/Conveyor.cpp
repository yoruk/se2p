#include <iostream>
#include "../HWaccess.h"
#include "../Global.h"
#include "Util.h"
#include "Conveyor.h"
#include "hw.h"

/// This class gives access to the Conveyor-belt
/// can move it in both direction (Left/Right) with a determine Direction (Fast/Slow) and also checks the status

static Mutex* mutex; /// the mutex for controlling the access
static Conveyor* conveyor; /// the conveyor object itself

/// Conveyor-constructor
Conveyor::Conveyor() {
	mutex = new Mutex();
}

/// Conveyor -deconstructor
Conveyor::~Conveyor() {
	delete mutex;
}

/// returns the only running instance of Conveyor
Conveyor* Conveyor::getInstance() {
	if (!init_HW_Done()) {
		init_HW();
	}

	if (!conveyor) {
		conveyor = new Conveyor();
	}

	return conveyor;
}

/// checks if Conveyor's direction is Left/Right
///
/// \return 		direction
///					0=left, 1=right
int Conveyor::getDirection() {
	if (bitIsSet((unsigned char*) DIO_A, CONVEYOR_RIGHT)) {
		return CONVEYOR_DIRECTION_RIGHT;
	} else {
		return CONVEYOR_DIRECTION_LEFT;
	}
}

/// checks speed of Conveyor
///
/// \return 		speed status
///					0=stopped, 1=slow, 2=fast
int Conveyor::getSpeed() {
	if (bitIsSet((unsigned char*) DIO_A, CONVEYOR_STOP)) {
		return CONVEYOR_STOPPED;
	} else if (bitIsSet((unsigned char*) DIO_A, CONVEYOR_SLOW)) {
		return CONVEYOR_RUNNING_SLOW;
	} else {
		return CONVEYOR_RUNNING_FAST;
	}
}

/// moves the conveyor to the right, after resetting the "move left" bit
void Conveyor::moveRight() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	unsetBit(&reg, CONVEYOR_LEFT);
	setBit(&reg, CONVEYOR_RIGHT);
	out8(DIO_A, reg);

	mutex->unlock();
}

/// moves the conveyor to the left, after resetting the "move right" bit
void Conveyor::moveLeft() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	unsetBit(&reg, CONVEYOR_RIGHT);
	setBit(&reg, CONVEYOR_LEFT);
	out8(DIO_A, reg);

	mutex->unlock();
}

/// sets the "move slow" bit, whether the conveyor-belt is moving or not
void Conveyor::moveSlow() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, CONVEYOR_SLOW);
	out8(DIO_A, reg);

	mutex->unlock();
}

/// resets the "move slow" bit, whether the conveyor-belt is moving or not
void Conveyor::moveFast() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	unsetBit(&reg, CONVEYOR_SLOW);
	out8(DIO_A, reg);

	mutex->unlock();
}

/// sets the "stop" bit and the conveyor stops whether other bits are sets or not
void Conveyor::conveyorStop() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, CONVEYOR_STOP);
	out8(DIO_A, reg);

	mutex->unlock();
}

/// resets the "stop" bit
void Conveyor::conveyorContinue() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	unsetBit(&reg, CONVEYOR_STOP);
	out8(DIO_A, reg);

	mutex->unlock();
}
