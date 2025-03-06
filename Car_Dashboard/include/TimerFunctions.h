#ifndef TIMER_FUNCTIONS_H
#define TIMER_FUNCTIONS_H 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

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
        bool running;
};

#endif