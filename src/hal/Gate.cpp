#include <iostream>
#include "../HWaccess.h"
#include "../Global.h"
#include "Util.h"
#include "Gate.h"
#include "hw.h"

static Mutex* mutex = new Mutex();
static Gate* gate;

Gate::Gate() {}

Gate::~Gate() {}

Gate* Gate::getInstance() {
	if(!init_HW_Done()) {
		init_HW();
	}

	if(!gate) {
		gate = new Gate();
	}

	return gate;
}

int Gate::status() {
	unsigned char reg = in8(DIO_A);
	return bitIsSet(&reg, GATE);
}

void Gate::close() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	unsetBit(&reg, GATE);
	out8(DIO_A, reg);

	mutex->unlock();
}

void Gate::open() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, GATE);
	out8(DIO_A, reg);

	mutex->unlock();
}
