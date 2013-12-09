#ifndef TIMERHANDLER_H_
#define TIMERHANDLER_H_

#include <vector>
#include <iostream.h>
#include <stdint.h>
#include "Timer.h"
#include "Mutex.h"

//#define DEBUG_TimerHandler

class TimerHandler {
public:
        /**
         * Standard destructor
         */
        virtual ~TimerHandler();

        /**
         * Function returns an instance of the TimerHandler-class. Class is
         * using singleton pattern, so only one instance for access to
         * TimerHandler exists.
         * @return instance for TimerHandler
         */
        static TimerHandler* getInstance();

        /**
         * Creates a new timer with the given values and adds the timer to the
         * timervector
         *
         * @param chid to send timeout msg to
         * @param seconds
         * @param milliseconds
         * @param msg sent with pulse on timeout
         * @return reference to the created timerobejct
         */
        Timer* createTimer(int chid, int sec, int msec, int msg);

        /**
         * Deletes the given timer, also from the timervector
         *
         * @paramm timer to delete
         */
        void deleteTimer(Timer* timer);

        /**
         * Calls pause()-function on all timers in the timervector
         */
        void pauseAllTimers();

        /**
         * Calls cont()-function on all timers in the timervector
         */
        void continueAllTimers();

        /**
         * Starts a give timer
         *
         * @param Timer to be started
         */
        void startTimer(Timer* timer);

        /**
         * Reset the TimerHandler, delete all items and reset paused-bool
         */
        void reset();

private:
        /**
         * default private constructor for singleton pattern.
         */
        TimerHandler();

        /**
         * instance for TimerHandler (singleton)
         */
        static TimerHandler* instance;

        /**
         * mutex for securing access to TimerHandler instance
         */
        static Mutex* instanceMutex;

        /**
         * Vector to save references to all created timers
         */
        vector<Timer*> timers;

        /**
         * Mutex to secure access to the timervector
         */
        Mutex* mutex;

        /**
         * Indicates if all timers are paused or not
         */
        bool paused;
};

#endif /* TIMERHANDLER_H_ */
