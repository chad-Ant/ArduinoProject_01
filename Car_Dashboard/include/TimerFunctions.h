#ifndef TIMER_FUNCTIONS_H
#define TIMER_FUNCTIONS_H 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

static constexpr unsigned long MIN_INTERVAL_MS = 1;
static constexpr unsigned long MAX_INTERVAL_MS = ULONG_MAX;

/**
 * @name getInterval (Helper function)
 * @brief Get the interval between two times
 * @return The interval between the two times
 */
inline unsigned long getInterval(unsigned long lastTime, unsigned long currentTime){
    return (currentTime >= lastTime ? currentTime - lastTime : MAX_INTERVAL_MS - lastTime + currentTime);
}

/**
 * @name isTimeout
 * @brief Check if a generic task has timed out
 * @return true if the task has timed out, false otherwise
 * @note the parameter startTime must be initialized using millis() before each use
 * @note example usage: startTime = millis(); while (!timeout(1000,startTime)) {do something}
 */
inline bool isTimeout(const unsigned long timer, unsigned long &startTime){
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
 * @name resetTask
 * @brief Reset the task by setting the last run time to millis(). That's it.
 */
inline void resetTask(unsigned long &startTime){
    startTime = millis();
}

bool getDate(uint8_t &_date, uint8_t &_month, uint16_t &_year, int8_t timezone = 0);
bool getTime(uint8_t &_hour, uint8_t &_minute, uint8_t &_second, int8_t timezone = 0);
bool setRTCDateTime(RTCZero &rtc, int8_t timezone = LOCAL_TIMEZONE);
bool setAlarmTime(RTCZero &rtc, uint8_t hour, uint8_t minute, uint8_t second);
void armAlarm(RTCZero &rtc, uint8_t alarmType, voidFuncPtr callback, bool enable);

#endif