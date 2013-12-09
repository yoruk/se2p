#ifndef TIMER_H_
#define TIMER_H_

#include <time.h>
#include <sys/neutrino.h>
#include "HWaccess.h"
#include "Global.h"

#define MILLISECONDS_NANOSECONDS_CONV        1000000
#define SECONDS_MILLISECONDS_CONV                1000

class Timer {
public:
        /**
         * Constructor creates a new timer, attaches to the given
         * channel.
         * Does not start timer!
         *
         * @param chid
         * @param seconds
         * @param milliseconds
         * @param msg
         */
        Timer(int chid, int sec, int msec, int msg);

        /**
         * Deletes timer after detach from channel
         */
        virtual ~Timer();

        /**
         * Starts the timer
         */
        void start();

        /**
         * Stops timer and resets it to initial values
         */
        void stop();

        /**
         * Pauses the timer
         */
        void pause();

        /**
         * Continues the timer
         */
        void tcontinue();

        /**
         * Resets the timer to initial values
         */
        void reset();

        /**
         * Changes timer values to new given values and
         * resets it
         * Does not start the timer!
         *
         * @param seconds
         * @param milliseconds
         */
        void changeTime(int sec, int msec);

        /**
         * Timer ID
         * TODO: nur public wg debug ausgaben, change that!
         */
        timer_t timerid;
private:

        /**
         * Timerstruct for running timer
         */
        struct itimerspec timer;

        /**
         * Timerstruct for backing up the running timer
         */
        struct itimerspec backupTimer;

        /**
         * timer value: seconds
         */
        int seconds;

        /**
         * timer value: milliseconds
         */
        int miliSeconds;

        /**
         * Connection ID for timeout pulse
         */
        int coid;

        /**
         * Event structure for timer if it fires
         */
        struct sigevent event;

        /**
         * Is set if timer was paused
         */
        bool isPaused;

        /**
         * Is set if timer was started
         */
        bool isStarted;
};

#endif /* TIMER_H_ */
