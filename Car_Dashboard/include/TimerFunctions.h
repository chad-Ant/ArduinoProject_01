#ifndef TIMER_FUNCTIONS_H
#define TIMER_FUNCTIONS_H 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

#define MIN_INTERVAL_MS     1UL
#define MAX_INTERVAL_MS     ULONG_MAX

enum class TimerReturnStatus{
    OK = 0,
    NOK_RTC_FAILED = -1,
    NOK_NO_CONNECTION = -2,
    NOK_NTP_TIME_FAILED = -3,
    NOK_SET_ALARM_FAILED = -4,
    NOK_VOID_CALLBACK = -5,
};

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
    return getInterval(startTime,millis()) >= timer;
}

/**
 * @name resetTask
 * @brief Reset the task by setting the last run time to millis(). That's it.
 */
inline void resetTask(unsigned long &startTime){
    startTime = millis();
}

TimerReturnStatus initializeRTC(RTCZero &rtc);
TimerReturnStatus getNTPDate(uint8_t &tDate, uint8_t &tMonth, uint16_t &tYear, int8_t timezone = 0);
TimerReturnStatus getNTPTime(uint8_t &tHour, uint8_t &tMinute, uint8_t &tSecond, int8_t timezone = 0);
TimerReturnStatus getNTPDateTime(uint8_t &tHour, uint8_t &tMinute, uint8_t &tSecond, uint8_t &tDate, uint8_t &tMonth, uint16_t &tYear, int8_t timezone = 0);
TimerReturnStatus setRTCDateTime(RTCZero &rtc, int8_t timezone = LOCAL_TIMEZONE);
void getRTCDate(RTCZero &rtc, uint8_t &tDate, uint8_t &tMonth, uint16_t &tYear);
void getRTCTime(RTCZero &rtc, uint8_t &tHour, uint8_t &tMinute, uint8_t &tSecond);
TimerReturnStatus setAlarmTime(RTCZero &rtc, uint8_t hour, uint8_t minute, uint8_t second);
TimerReturnStatus armAlarm(RTCZero &rtc, RTCZero::Alarm_Match alarmType, voidFuncPtr callback, bool enable);

#endif