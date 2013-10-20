#include <iostream>
#include "../HWaccess.h"
#include "../Global.h"
#include "../Mutex.h"
#include "Util.h"
#include "Led.h"
#include "hw.h"

static Mutex* mutex;
static Led* led;

Led::Led() {
	mutex = new Mutex();
}

Led::~Led() {
	delete mutex;
}

Led* Led::getInstance() {
	if(!init_HW_Done()) {
		init_HW();
	}

	if(!led) {
		led = new Led();
	}

	return led;
}

void Led::led_StartButton_On() {
	mutex->lock();

	unsigned char reg = in8(DIO_C);
	setBit(&reg, LED_START_BUTTON);
	out8(DIO_C, reg);

	mutex->unlock();
}

void Led::led_StartButton_Off() {
	mutex->lock();

	unsigned char reg = in8(DIO_C);
	unsetBit(&reg, LED_START_BUTTON);
	out8(DIO_C, reg);

	mutex->unlock();
}

void Led::led_ResetButton_On() {
	mutex->lock();

	unsigned char reg = in8(DIO_C);
	setBit(&reg, LED_RESET_BUTTON);
	out8(DIO_C, reg);

	mutex->unlock();
}

void Led::led_ResetButton_Off() {
	mutex->lock();

	unsigned char reg = in8(DIO_C);
	unsetBit(&reg, LED_RESET_BUTTON);
	out8(DIO_C, reg);

	mutex->unlock();
}

void Led::led_Q1_On() {
	mutex->lock();

	unsigned char reg = in8(DIO_C);
	setBit(&reg, LED_Q1);
	out8(DIO_C, reg);

	mutex->unlock();
}

void Led::led_Q1_Off() {
	mutex->lock();

	unsigned char reg = in8(DIO_C);
	unsetBit(&reg, LED_Q1);
	out8(DIO_C, reg);

	mutex->unlock();
}

void Led::led_Q2_On() {
	mutex->lock();

	unsigned char reg = in8(DIO_C);
	setBit(&reg, LED_Q2);
	out8(DIO_C, reg);

	mutex->unlock();
}

void Led::led_Q2_Off() {
	mutex->lock();

	unsigned char reg = in8(DIO_C);
	unsetBit(&reg, LED_Q2);
	out8(DIO_C, reg);

	mutex->unlock();
}
