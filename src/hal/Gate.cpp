#include <iostream>
#include "../HWaccess.h"
#include "../Global.h"
#include "Util.h"
#include "Gate.h"
#include "hw.h"

/// This class gives access to the gates
/// can open/close the gate and also checks the status

static Mutex* mutex; /// the mutex for controlling the access
static Gate* gate; /// the gate object itself

/// Gate-constructor
Gate::Gate() {
	mutex = new Mutex();
}

/// Gate-deconstructor
Gate::~Gate() {
	delete mutex;
}

/// returns the only running instance of Gate
Gate* Gate::getInstance() {
	mutex->lock();

	if(!init_HW_Done()) {
		init_HW();
	}

	if(!gate) {
		gate = new Gate();
	}

	mutex->unlock();

	return gate;
}

/// checks if gate is open/closed
///
/// \return		the status of the gate,
///				0=closed, 1=open
int Gate::status() {
	unsigned char reg = in8(DIO_A);
	return bitIsSet(&reg, GATE);
}

/// closes the gate
void Gate::close() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	unsetBit(&reg, GATE);
	out8(DIO_A, reg);

	mutex->unlock();
}

/// opens the gate
void Gate::open() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, GATE);
	out8(DIO_A, reg);

	mutex->unlock();
}
