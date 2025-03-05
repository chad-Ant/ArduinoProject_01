#include "../include/TimerFunctions.h"

class Timer{
    unsigned long m_setTime;
    bool autoReset;
    bool expired;

    public:
        Timer(unsigned long time, bool autoReset);
        ~Timer();
        bool startTimer();
        void stopTimer();
        void resetTimer();

        void setTimer(unsigned long time);
        void setAutoReset(bool autoReset);

        unsigned long getTimer() const;
        bool isAutoReset() const;
        unsigned long getElapsedTime() const;
        unsigned long getRemainingTime() const;
        bool isExpired();
    private:
        unsigned long timer(bool reset);
        unsigned long zTime;
};

Timer::Timer(unsigned long time, bool autoReset)
    m_setTime(time),
    autoReset(autoReset),
    expired(true){
    timer(true);
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
    return timer(expired) >= m_setTime; 
}

void Timer::stopTimer(){
    expired = timer(true) >= m_setTime;
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

unsigned long Timer::getElapsedTime(){
    return timer(false);
}

unsigned long Timer::getRemainingTime(){
    unsigned long elapsedTime = timer(false);
    return m_setTime >= elapsedTime ? m_setTime - elapsedTime : 0;
}

bool Timer::isExpired(){
    if (timer(false) >= m_setTime){
        if (autoReset){
            resetTimer();
        } else {
            expired = true;
        }
        return true;
    }
    return false;
}