#include "../include/TimerFunctions.h"

/**
 * @name timeout
 * @brief Check if a generic task has timed out
 * @return true if the task has timed out, false otherwise
 */
inline bool timeout(const unsigned long timer, unsigned long &lastRun){
    if (timer < MIN_INTERVAL_MS){
        return true;
    }
    unsigned long currentTime = millis();
    if (getInterval(lastRun,currentTime) >= timer){
        lastRun = currentTime;
        return true;
    }
    return false;
}

/**
 * @name taskScheduler
 * @brief Check if a defined task is allowed to run
 * @return true if the task is allowed to run, false otherwise
 */
inline bool taskScheduler(const TaskSchedule &task, Task taskName, unsigned long &lastRun){
    if (task.interval[taskName] < MIN_INTERVAL_MS || !task.enabled[taskName]){
        return false;
    }
    unsigned long currentTime = millis();
    if (getInterval(lastRun[taskName],currentTime) >= task.interval[taskName]){
        lastRun[taskName] = currentTime;
        return true;
    }
    return false;
}

/**
 * @name resetTask
 * @brief Reset the task by setting the last run time to 0. That's it.
 */
inline void resetTask(Task taskName, unsigned long &lastRun){
    lastRun[taskName] = 0;
}

/**
 * @name getInterval (Helper function)
 * @brief Get the interval between two times
 * @return The interval between the two times
 */
inline unsigned long getInterval(unsigned long lastTime, unsigned long currentTime){
    return (currentTime >= lastTime ? currentTime - lastTime : MAX_INTERVAL_MS - lastTime + currentTime);
}