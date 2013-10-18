#include <iostream>
#include "../HWaccess.h"
#include "../Global.h"
#include "Util.h"
#include "Treadmill.h"
#include "hw.h"

static Mutex* mutex = new Mutex();
static Treadmill* treadmill;

Treadmill::Treadmill() {}

Treadmill::~Treadmill() {}

Treadmill* Treadmill::getInstance(){
	if (!init_HW_Done()) {
		init_HW();
	}

	if (!treadmill) {
		treadmill = new Treadmill();
	}

	return treadmill;
}

int status(unsigned int treadmill){
	return bitIsSet((unsigned char*) DIO_A, treadmill);
}

void moveRight(){
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, RIGHT);
	out8(DIO_A, reg);

	mutex->unlock();
}

void moveLeft(){
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, LEFT);
	out8(DIO_A, reg);

	mutex->unlock();
}

void moveSlow(){
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, SLOW);
	out8(DIO_A, reg);

	mutex->unlock();
}

void treadmilStop(){
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, STOP);
	out8(DIO_A, reg);

	mutex->unlock();
}
