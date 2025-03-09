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

inline bool timeout(const unsigned long timer, unsigned long &lastRun);
inline bool taskScheduler(const TaskSchedule &task, Task taskName, unsigned long &lastRun);
inline void resetTask(Task taskName, unsigned long &lastRun);

#endif