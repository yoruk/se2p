#include <iostream>
#include "../HWaccess.h"
#include "../Global.h"
#include "Util.h"
#include "Gate.h"
#include "hw.h"

using namespace std;

static Mutex* mutex;
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
	return bitIsSet((unsigned char*)DIO_A, BIT_4);
}

void Gate::close() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	unsetBit(&reg, BIT_4);
	out8(DIO_A, reg);

	mutex->unlock();
}

void Gate::open() {
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, BIT_4);
	out8(DIO_A, reg);

	mutex->unlock();
}



