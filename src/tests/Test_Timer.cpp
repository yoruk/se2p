#include "Test_Timer.h"

void test_Timer_start() {
	struct _pulse pulse;
	int chid = ChannelCreate(0);

	TimerHandler* timers = TimerHandler::getInstance();

	Timer* timer1;
	Timer* timer2;
	Timer* timer3;

	timer1 = timers->createTimer(chid, 10, 0, 1);


	timers->startTimer(timer1);

	timers->deleteTimer(timer1);
	bool start = true;
	while (start) {
		MsgReceivePulse(chid, &pulse, sizeof(pulse), NULL);
		if (pulse.code == PULSE_FROM_TIMER && pulse.value.sival_int == 1) {
			printf("recved timer 2sec timeout pulse\n");
			fflush(stdout);
		}
	}

	timers->deleteTimer(timer1);



	timer1 = timers->createTimer(chid, 2, 0, 1);
	timer2 = timers->createTimer(chid, 4, 0, 2);
	timer3 = timers->createTimer(chid, 6, 0, 3);

	printf("start");fflush(stdout);
	timers->startTimer(timer1);
	timers->startTimer(timer2);
	timers->startTimer(timer3);

	timers->pauseAllTimers();
	sleep(3);
	timers->continueAllTimers();

	start = true;

	while (start) {
		MsgReceivePulse(chid, &pulse, sizeof(pulse), NULL);
		if (pulse.code == PULSE_FROM_TIMER && pulse.value.sival_int == 1) {
			printf("recved timer 5sec timeout pulse\n");
			fflush(stdout);
			timer2->reset();
			timer2->start();
			timer3->pause();
		}

		if (pulse.code == PULSE_FROM_TIMER && pulse.value.sival_int == 2) {
			printf("recved timer 9sec timeout pulse\n");
			fflush(stdout);
			timer3->tcontinue();
			timer3->changeTime(8,0);
			timer3->start();

		}

		if (pulse.code == PULSE_FROM_TIMER && pulse.value.sival_int == 3) {
			printf("recved timer 17sec timeout pulse\n");
			fflush(stdout);
			start = false;
		}
	}

}
