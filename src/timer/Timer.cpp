#include "Timer.h"
#include <errno.h>

Timer::Timer(int chid, int sec, int msec, int msg) {
        if ((coid = ConnectAttach(0, 0, chid, _NTO_SIDE_CHANNEL, 0)) == -1) {
                printf("Timer: Error in ConnectAttach\n");
        }

        SIGEV_PULSE_INIT(&event, coid, SIGEV_PULSE_PRIO_INHERIT, PULSE_FROM_TIMER, msg/*Timer abgelaufen*/);

        if (timer_create(CLOCK_REALTIME, &event, &timerid) == -1) {
                printf("Timer: Error in timer_create()\n");
        }

        seconds = sec;
        miliSeconds = msec;
        isPaused = false;

        reset();
}

Timer::~Timer() {
        if (ConnectDetach(coid) == -1) {
                printf("Timer: Error in ConnectDetach\n");
        }

        if (timer_delete(timerid) == -1) {
                printf("Timer: Error in timer_delete()\n");
        }
}

void Timer::start() {
        if (!isStarted) {
                if (timer_settime(timerid, 0, &timer, NULL) == -1) {
                        printf("Timer: Error in start() timer_settime()\n");
                }
                timer.it_value.tv_sec = 0;
                timer.it_value.tv_nsec = 0;
                timer.it_interval.tv_sec = 0;
                timer.it_interval.tv_nsec = 0;
                isStarted = true;
        }
}

void Timer::stop() {
        // Stop Timer
        if(timer_settime(timerid, 0, &timer, NULL) == -1){
                printf("Timer: Error in stop() timer_settime(), errno:%d\n", errno);
        }
        //Timer reset
        reset();
}

void Timer::pause() {
        if (!isPaused) {
                if (timer_settime(timerid, 0, &timer, &backupTimer) == -1) {
                        printf("Timer: Error in pause() timer_settime(), errno:%d\n", errno);
                }
                isPaused = true;
        }
}

void Timer::tcontinue() {
        if (isPaused) {
                if (timer_settime(timerid, 0, &backupTimer, NULL) == -1) {
                        printf("Timer: Error in cont() timer_settime(), errno:%d\n", errno);
                }
                isPaused = false;
        }
}

void Timer::reset(){
        timer.it_value.tv_sec = seconds;
        timer.it_value.tv_nsec = miliSeconds * MILLISECONDS_NANOSECONDS_CONV;
        timer.it_interval.tv_sec = 0;
        timer.it_interval.tv_nsec = 0;
        isStarted = false;
}

void Timer::changeTime(int sec, int msec){
        seconds = sec;
        miliSeconds = msec;
        stop();
}
