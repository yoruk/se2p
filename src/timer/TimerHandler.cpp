#include "TimerHandler.h"

TimerHandler* TimerHandler::instance = NULL;
Mutex* TimerHandler::instanceMutex = new Mutex();

TimerHandler::TimerHandler() {
        mutex = new Mutex();
}

TimerHandler::~TimerHandler() {
        if (instance != NULL) {
                delete instance;
                instance = NULL;
                instanceMutex->~Mutex();
        }
}

TimerHandler* TimerHandler::getInstance()
{
        if (!instance) {
                instanceMutex->lock();
                if (!instance) {
                        instance = new TimerHandler;
#ifdef DEBUG_TimerHandler
                        printf("Debug TimerHandler: New TimerHandler instance created\n");
#endif
                }
                instanceMutex->unlock();
        }
        return instance;
}

Timer* TimerHandler::createTimer(int chid, int sec, int msec, int msg) {
        mutex->lock();
        Timer* tmpTimer = new Timer(chid, sec, msec, msg);
        timers.push_back(tmpTimer);
        mutex->unlock();
#ifdef DEBUG_TimerHandler
        printf("Debug TimerHandler: New Timer created with id: %d, vector size: %d\n", tmpTimer->timerid, timers.size());
#endif
        return tmpTimer;
}

void TimerHandler::startTimer(Timer* timer){
        timer->start();
        if(paused){
                timer->pause();
        }
}

void TimerHandler::deleteTimer(Timer* timer) {
        mutex->lock();
        if(timer != NULL){
#ifdef DEBUG_TimerHandler
        printf("Debug TimerHandler: delete called on timerid %d\n",timer->timerid);
#endif
                if (!timers.empty()) {
                        timer->stop();
                        for (uint32_t i = 0; i < timers.size(); i++) {
                                if (timers.at(i) == timer) {
                                        timers.erase(timers.begin() + i);
                                        delete timer;
                                        timer = NULL;
                                        //printf("Debug TimerHandler: for-loop: deleted 1 timer\n");
                                        break;
                                }
                        }
                }
        }
        mutex->unlock();
}

void TimerHandler::pauseAllTimers(){
        mutex->lock();
        paused = true;
        //printf("Debug timerhandler: anzahl timer: %d\n", timers.size());
        if (!timers.empty()) {
                for (uint32_t i = 0; i < timers.size(); i++) {
                        timers.at(i)->pause();
#ifdef DEBUG_TimerHandler
                        printf("Debug TimerHandler: for-loo: paused 1 Timer\n");
#endif
                }
        }
        mutex->unlock();
}

void TimerHandler::continueAllTimers(){
        mutex->lock();
        if (!timers.empty()) {
                for (uint32_t i = 0; i < timers.size(); i++) {
                        timers.at(i)->tcontinue();
                }
        }
        paused = false;
        mutex->unlock();
}

void TimerHandler::reset(){
        paused = false;
        if(!timers.empty()){
                timers.clear();
        }
}
