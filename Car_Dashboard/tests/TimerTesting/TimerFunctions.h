#ifndef TIMER_FUNCTIONS_H
#define TIMER_FUNCTIONS_H 1

#include <Arduino.h>
#include <climits>

enum class Task{
    GPS,
    SERVO,
    SEGLED,
    HTTP,
    WIFI,
    CAN,
    COMMS,
    TASK_COUNT // This is a dummy task to get the number of tasks
};

const uint8_t taskCount = static_cast<uint8_t>(Task::TASK_COUNT);

struct TaskSchedule{
    Task task[taskCount];
    unsigned long interval[taskCount];
    bool enabled[taskCount];
};

static constexpr unsigned long MIN_INTERVAL_MS = 1;
static constexpr unsigned long MAX_INTERVAL_MS = ULONG_MAX;

extern unsigned long taskLastRun[taskCount];    //to be declared in Car_Dashboard.ino

/**
 * @name getInterval (Helper function)
 * @brief Get the interval between two times
 * @return The interval between the two times
 */
inline unsigned long getInterval(unsigned long lastTime, unsigned long currentTime){
    return (currentTime >= lastTime ? currentTime - lastTime : MAX_INTERVAL_MS - lastTime + currentTime);
}

/**
 * @name timeout
 * @brief Check if a generic task has timed out
 * @return true if the task has timed out, false otherwise
 * @note the parameter startTime must be initialized using millis() before each use
 * @note example usage: startTime = millis(); while (!timeout(1000,startTime)) {do something}
 */
inline bool hasTimeElapsed(const unsigned long timer, unsigned long &startTime){
    if (timer < MIN_INTERVAL_MS){
        return true;
    }
    unsigned long currentTime = millis();
    if (getInterval(startTime,currentTime) >= timer){
        return true;
    }
    return false;
}

/**
 * @name taskScheduler
 * @brief Check if a defined task is allowed to run
 * @return true if the task is allowed to run, false otherwise
 */
inline bool taskScheduler(const TaskSchedule &task, Task taskName, unsigned long (&lastRun)[taskCount]){
    int taskNameInt = static_cast<int>(taskName);
    if (task.interval[taskNameInt] < MIN_INTERVAL_MS || !task.enabled[taskNameInt]){
        return false;
    }
    unsigned long currentTime = millis();
    if (getInterval(lastRun[taskNameInt],currentTime) >= task.interval[taskNameInt]){
        lastRun[taskNameInt] = currentTime;
        return true;
    }
    return false;
}

/**
 * @name resetTask
 * @brief Reset the task by setting the last run time to 0. That's it.
 */
inline void resetTask(Task taskName, unsigned long (&lastRun)[taskCount]){
    int taskNameInt = static_cast<int>(taskName);
    lastRun[taskNameInt] = millis();
}

#endif