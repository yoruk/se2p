#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../Global.h"
#include "../hal/Led.h"
#include "../hal/TrafficLight.h"
#include "../hal/Gate.h"
#include "../hal/Conveyor.h"

#define WAIT 2
#define LONG_WAIT 5

void led_test() {
	Led* led = Led::getInstance();

	printf("\n::testing led::\n");fflush(stdout);

	// on
	printf("switching on: start-button-led\n");fflush(stdout);
	led->led_StartButton_On();
	sleep(WAIT);

	printf("switching on: reset-button-led\n");fflush(stdout);
	led->led_ResetButton_On();
	sleep(WAIT);

	printf("switching on: q1-led\n");fflush(stdout);
	led->led_Q1_On();
	sleep(WAIT);

	printf("switching on: q2-led\n");fflush(stdout);
	led->led_Q2_On();
	sleep(WAIT);

	// off
	printf("switching off: start-button-led\n");fflush(stdout);
	led->led_StartButton_Off();
	sleep(WAIT);

	printf("switching off: reset-button-led\n");fflush(stdout);
	led->led_ResetButton_Off();
	sleep(WAIT);

	printf("switching off: q1-led\n");fflush(stdout);
	led->led_Q1_Off();
	sleep(WAIT);

	printf("switching off: q2-led\n");fflush(stdout);
	led->led_Q2_Off();
	sleep(WAIT);
}

void trafficlight_test() {
	TrafficLight* tlight = TrafficLight::getInstance();

	printf("\n::testing traffic light::\n");fflush(stdout);

	// on
	printf("switching on: red-light\n");fflush(stdout);
	tlight->redOn();
	sleep(WAIT);

	printf("switching on: yellow-light\n");fflush(stdout);
	tlight->yellowOn();
	sleep(WAIT);

	printf("switching on: green-light\n");fflush(stdout);
	tlight->greenOn();
	sleep(WAIT);

	// off
	printf("switching off: red-light\n");fflush(stdout);
	tlight->redOff();
	sleep(WAIT);

	printf("switching off: yellow-light\n");fflush(stdout);
	tlight->yellowOff();
	sleep(WAIT);

	printf("switching off: green-light\n");fflush(stdout);
	tlight->greenOff();
	sleep(WAIT);
}

void gate_test() {
	Gate* gate = Gate::getInstance();

	printf("\n::testing gate::\n");fflush(stdout);

	// open
	printf("opening gate\n");fflush(stdout);
	gate->open();
	sleep(WAIT);

	printf("closing gate\n");fflush(stdout);
	gate->close();
	sleep(WAIT);

	printf("opening gate\n");fflush(stdout);
	gate->open();
	sleep(WAIT);

	printf("closing gate\n");fflush(stdout);
	gate->close();
	sleep(WAIT);
}

void conveyor_test() {
	Conveyor* conveyor = Conveyor::getInstance();

	printf("\n::testing conveyor::\n");fflush(stdout);

	// right
	printf("conveyor: start moving, direction right, slow speed\n");fflush(stdout);
	conveyor->moveSlow();
	conveyor->moveRight();
	sleep(LONG_WAIT);

	printf("conveyor: normal speed\n");fflush(stdout);
	conveyor->moveFast();
	sleep(LONG_WAIT);

	// stop
	printf("conveyor: stop\n");fflush(stdout);
	conveyor->conveyorStop();
	sleep(LONG_WAIT);

	// continue
	printf("conveyor: continue moving\n");fflush(stdout);
	conveyor->conveyorContinue();
	sleep(LONG_WAIT);

	// left
	printf("conveyor: reverse, direction left, fast speed\n");fflush(stdout);
	conveyor->moveLeft();
	sleep(LONG_WAIT);

	printf("conveyor: slowing down\n");fflush(stdout);
	conveyor->moveSlow();
	sleep(LONG_WAIT);

	// stop
	printf("conveyor: stopping\n");fflush(stdout);
	conveyor->conveyorStop();
	sleep(LONG_WAIT);
}

void simple_milestone2_start() {
	printf("simple_milestone12_start() is running\n");fflush(stdout);

	led_test();
	trafficlight_test();
	gate_test();
	conveyor_test();
}
