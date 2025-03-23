#include "../include/TimerFunctions.h"
#include "../include/WiFiFunctions.h"

bool getDate(uint8_t &_date, uint8_t &_month, uint16_t &_year, int8_t timezone){ //areas with fractional timezone offsets not supported
    if (!isWifiConnected()) return false;
    unsigned long unixTime = getUnixTime();
    if (unixTime == 0) return false;
    checkTimezoneValidity(timezone);
    unixTime += timezone * 3600;
    time_t t = unixTime;
    _year = year(t);
    _month = month(t);
    _date = day(t);
    return true;
}

bool getTime(uint8_t &_hour, uint8_t &_minute, uint8_t &_second, int8_t timezone){
    if (!isWifiConnected()) return false;
    unsigned long unixTime = getUnixTime();
    if (unixTime == 0) return false;
    checkTimezoneValidity(timezone);
    unixTime += timezone * 3600;
    time_t t = unixTime;
    _hour = hour(t);
    _minute = minute(t);
    _second = second(t);
    return true;
}

bool setRTCDateTime(RTCZero &rtc, int8_t timezone){
    if (!isWifiConnected()) return false;
    unsigned long unixTime = getUnixTime();
    if (unixTime == 0) return false;
    checkTimezoneValidity(timezone);
    unixTime += timezone * 3600;
    time_t t = unixTime;
    rtc.setTime(hour(t), minute(t), second(t));
    rtc.setDate(day(t), month(t), year(t));
    return true;
}

void setAlarmTime(RTCZero &rtc, uint8_t hour, uint8_t minute, uint8_t second){
    checkTimeValidity(hour, minute, second);
    rtc.setAlarmTime(hour, minute, second);
}

void armAlarm(RTCZero &rtc, RTCZero::Alarm_Match alarmType, voidFuncPtr callback, bool enable){
    if callback == nullptr return;
    if (enable){
        rtc.enableAlarm(alarmType);
        rtc.attachInterrupt(callback);
    } else {
        rtc.detachInterrupt();
        rtc.disableAlarm();
    }
}

void checkTimeValidity(uint8_t &hour, uint8_t &minute, uint8_t &second){
    hour = hour > 23 ? 23 : hour;
    minute = minute > 59 ? 59 : minute;
    second = second > 59 ? 59 : second;
}

void checkTimezoneValidity(int8_t &timezone){
    timezone = timezone < -12 ? -12 : (timezone > 12 ? 12 : timezone);
}