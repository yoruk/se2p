#include <iostream>
#include "../HWaccess.h"
#include "../Global.h"
#include "Util.h"
#include "Conveyor.h"
#include "hw.h"

static Mutex* mutex = new Mutex();
static Conveyor* conveyor;

Conveyor::Conveyor() {}

Conveyor::~Conveyor() {}

Conveyor* Conveyor::getInstance(){
	if (!init_HW_Done()) {
		init_HW();
	}

	if (!conveyor) {
		conveyor = new Conveyor();
	}

	return conveyor;
}

int Conveyor::statusRight(){
	return bitIsSet((unsigned char*) DIO_A, RIGHT);
}

int Conveyor::statusLeft(){
	return bitIsSet((unsigned char*) DIO_A, LEFT);
}

int Conveyor::statusSlow(){
	return bitIsSet((unsigned char*) DIO_A, SLOW);
}

int Conveyor::statusStop(){
	return bitIsSet((unsigned char*) DIO_A, STOP);
}

void Conveyor::moveRight(){
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, RIGHT);
	out8(DIO_A, reg);

	mutex->unlock();
}

void Conveyor::moveLeft(){
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, LEFT);
	out8(DIO_A, reg);

	mutex->unlock();
}

void Conveyor::moveSlow(){
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, SLOW);
	out8(DIO_A, reg);

	mutex->unlock();
}

void Conveyor::conveyorStop(){
	mutex->lock();

	unsigned char reg = in8(DIO_A);
	setBit(&reg, STOP);
	out8(DIO_A, reg);

	mutex->unlock();
}
