#include "../include/TimerFunctions.h"

inline bool timeout(unsigned long timer,unsigned long &lastTime){
    if (timer < 1){
        return true;
    }
    unsigned long currentTime = millis();
    if (getInterval(lastTime,currentTime) >= timer){
        lastTime = currentTime;
        return true;
    }
    return false;
}

inline bool taskScheduler(TaskSchedule &task){
    if (!task.enabled || task.interval < 1){
        return false;
    }
    unsigned long currentTime = millis();
    if (getInterval(task.lastRun,currentTime) >= task.interval){
        task.lastRun = currentTime;
        return true;
    }
    return false;
}

inline void resetTask(TaskSchedule &task){
    task.lastRun = 0;
}

inline unsigned long getInterval(unsigned long lastTime, unsigned long currentTime){
    return (currentTime >= lastTime ? currentTime - lastTime : ULONG_MAX - lastTime + currentTime);
}