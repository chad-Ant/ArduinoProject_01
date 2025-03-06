#include "../include/TimerFunctions.h"

Timer::Timer(unsigned long time, bool autoReset):
    m_setTime(time),
    autoReset(autoReset),
    expired(true),
    zTime(millis()){
    //do nothing
    }

Timer::~Timer(){
    //do nothing
}

unsigned long Timer::timer(bool reset){
    unsigned long cTime = millis();
    if (reset){
        zTime = cTime;
        return 0;
    }
    return cTime >= zTime ? cTime - zTime : ULONG_MAX - zTime + cTime;
}

bool Timer::startTimer(){
    if (expired){
        timer(true);
        expired = false;
        return true;
    }
    return false;
}

void Timer::stopTimer(){
    expired = true;
}

void Timer::resetTimer(){
    timer(true);
    expired = false;
}

void Timer::setTimer(unsigned long time){
    m_setTime = time;
}

void Timer::setAutoReset(bool autoReset){
    this->autoReset = autoReset;
}

unsigned long Timer::getTimer() const {
    return m_setTime;
}

bool Timer::isAutoReset() const {
    return autoReset;
}

unsigned long Timer::getElapsedTime() const {
    return timer(false);
}

unsigned long Timer::getRemainingTime() const {
    unsigned long elapsedTime = timer(false);
    return m_setTime >= elapsedTime ? m_setTime - elapsedTime : 0;
}

bool Timer::isExpired(){
    if (timer(false) >= m_setTime || !running){
        if (autoReset){
            timer(true);
            expired = false;
        } else {
            expired = true;
        }
        return true;
    }
    return false;
}