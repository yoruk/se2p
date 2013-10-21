#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../Global.h"
#include "../hal/Led.h"
#include "../hal/TrafficLight.h"
#include "../hal/Gate.h"
#include "../hal/Conveyor.h"

#define WAIT 2
#define LONG_WAIT 10

void led_test() {
	Led* led = Led::getInstance();

	printf("\n::testing led::\n");fflush(stdout);

	// on
	printf("switching on: start-button-led\n");fflush(stdout);
	sleep(WAIT);
	led->led_StartButton_On();
	printf("start-button-led is switched on\n");fflush(stdout);

	printf("switching on: reset-button-led\n");fflush(stdout);
	sleep(WAIT);
	led->led_ResetButton_On();
	printf("reset-button-led is switched on\n");fflush(stdout);

	printf("switching on: q1-led\n");fflush(stdout);
	sleep(WAIT);
	led->led_Q1_On();
	printf("q1-led is switched on\n");fflush(stdout);

	printf("switching on: q2-led\n");fflush(stdout);
	sleep(WAIT);
	led->led_Q2_On();
	printf("q2-led is switched on\n");fflush(stdout);

	// off
	printf("switching off: start-button-led\n");fflush(stdout);
	sleep(WAIT);
	led->led_StartButton_Off();
	printf("start-button-led is switched off\n");fflush(stdout);

	printf("switching off: reset-button-led\n");fflush(stdout);
	sleep(WAIT);
	led->led_ResetButton_Off();
	printf("reset-button-led is switched off\n");fflush(stdout);

	printf("switching off: q1-led\n");fflush(stdout);
	sleep(WAIT);
	led->led_Q1_Off();
	printf("q1-led is switched off\n");fflush(stdout);

	printf("switching off: q2-led\n");fflush(stdout);
	sleep(WAIT);
	led->led_Q2_Off();
	printf("q2-led is switched off\n");fflush(stdout);
}

void trafficlight_test() {
	TrafficLight* tlight = TrafficLight::getInstance();

	printf("\n::testing traffic light::\n");fflush(stdout);

	// on
	printf("switching on: red-light\n");fflush(stdout);
	sleep(WAIT);
	tlight->redOn();
	printf("red-led is switched on\n");fflush(stdout);

	printf("switching on: yellow-light\n");fflush(stdout);
	sleep(WAIT);
	tlight->yellowOn();
	printf("yellow-light is switched on\n");fflush(stdout);

	printf("switching on: green-light\n");fflush(stdout);
	sleep(WAIT);
	tlight->greenOn();
	printf("green-light is switched on\n");fflush(stdout);

	// off
	printf("switching off: red-light\n");fflush(stdout);
	sleep(WAIT);
	tlight->redOff();
	printf("red-led is switched off\n");fflush(stdout);

	printf("switching off: yellow-light\n");fflush(stdout);
	sleep(WAIT);
	tlight->yellowOff();
	printf("yellow-light is switched of\n");fflush(stdout);

	printf("switching off: green-light\n");fflush(stdout);
	sleep(WAIT);
	tlight->greenOff();
	printf("green-light is switched off\n");fflush(stdout);
}

void gate_test() {
	Gate* gate = Gate::getInstance();

	printf("\n::testing gate::\n");fflush(stdout);

	// open
	printf("opening gate:\n");fflush(stdout);
	sleep(WAIT);
	gate->open();
	printf("gate opened\n");fflush(stdout);

	printf("closing gate:\n");fflush(stdout);
	sleep(WAIT);
	gate->close();
	printf("gate closed\n");fflush(stdout);

	printf("opening gate:\n");fflush(stdout);
	sleep(WAIT);
	gate->open();
	printf("gate opened\n");fflush(stdout);

	printf("closing gate:\n");fflush(stdout);
	sleep(WAIT);
	gate->close();
	printf("gate closed\n");fflush(stdout);
}

void conveyor_test() {
	Conveyor* conveyor = Conveyor::getInstance();

	printf("\n::testing conveyor::\n");fflush(stdout);

	// right
	printf("conveyor: start moving, direction right, slow speed\n");fflush(stdout);
	sleep(LONG_WAIT);
	conveyor->moveSlow();
	conveyor->moveRight();
	printf("conveyor is moving\n");fflush(stdout);

	printf("conveyor: normal speed\n");fflush(stdout);
	sleep(LONG_WAIT);
	conveyor->moveFast();
	printf("conveyor is moving fast\n");fflush(stdout);

	// stop
	printf("conveyor: stop\n");fflush(stdout);
	sleep(LONG_WAIT);
	conveyor->conveyorStop();
	printf("conveyor stopped\n");fflush(stdout);

	// continue
	printf("conveyor: continue moving\n");fflush(stdout);
	sleep(LONG_WAIT);
	conveyor->conveyorContinue();
	printf("conveyor continued\n");fflush(stdout);

	// left
	printf("conveyor: reverse, direction left, fast speed\n");fflush(stdout);
	sleep(LONG_WAIT);
	conveyor->moveLeft();
	printf("conveyor reversed\n");fflush(stdout);

	printf("conveyor: slowing down\n");fflush(stdout);
	sleep(LONG_WAIT);
	conveyor->moveSlow();
	printf("conveyor slowed down\n");fflush(stdout);

	// stop
	printf("conveyor: stopping\n");fflush(stdout);
	sleep(LONG_WAIT);
	conveyor->conveyorStop();
	printf("conveyor stopped\n");fflush(stdout);
}

void simple_milestone2_start() {
	printf("simple_milestone12_start() is running\n");fflush(stdout);

	led_test();
	trafficlight_test();
	gate_test();
	conveyor_test();
}
