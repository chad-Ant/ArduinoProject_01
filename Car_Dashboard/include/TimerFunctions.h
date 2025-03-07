#ifndef TIMER_FUNCTIONS_H
#define TIMER_FUNCTIONS_H 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

enum class Task{
    GPS,
    SERVO,
    SEGLED,
    HTTP,
    WIFI,
    CAN,
    COMMS
};

struct TaskSchedule{
    Task task;
    unsigned long lastRun;
    unsigned long interval;
    bool enabled;
}

inline bool taskScheduler(TaskSchedule &task);
inline void resetTask(TaskSchedule &task);

#endif