#include "../include/TimerFunctions.h"
#include "../include/WiFiFunctions.h"

void checkTimeValidity(uint8_t &tHour, uint8_t &tMinute, uint8_t &tSecond){
    tHour = tHour > 23 ? 23 : tHour;
    tMinute = tMinute > 59 ? 59 : tMinute;
    tSecond = tSecond > 59 ? 59 : tSecond;
}

void checkTimezoneValidity(int8_t &timezone){
    timezone = timezone < -12 ? -12 : (timezone > 12 ? 12 : timezone);
}

bool getNTPDate(uint8_t &tDate, uint8_t &tMonth, uint16_t &tYear, int8_t timezone){ //areas with fractional timezone offsets not supported
    if (!isWifiConnected()) return false;
    unsigned long unixTime = 0;
    if (!getUnixTime(unixTime)) return false;
    checkTimezoneValidity(timezone);
    unixTime += timezone * 3600;
    time_t t = unixTime;
    tYear = year(t);
    tMonth = month(t);
    tDate = day(t);
    return true;
}

bool getNTPTime(uint8_t &tHour, uint8_t &tMinute, uint8_t &tSecond, int8_t timezone){
    if (!isWifiConnected()) return false;
    unsigned long unixTime = 0;
    if (!getUnixTime(unixTime)) return false;
    checkTimezoneValidity(timezone);
    unixTime += timezone * 3600;
    time_t t = unixTime;
    tHour = hour(t);
    tMinute = minute(t);
    tSecond = second(t);
    return true;
}

bool getNTPDateTime(uint8_t &tHour, uint8_t &tMinute, uint8_t &tSecond, uint8_t &tDate, uint8_t &tMonth, uint16_t &tYear, int8_t timezone){
    if (!isWifiConnected()) return false;
    unsigned long unixTime = 0;
    if (!getUnixTime(unixTime)) return false;
    checkTimezoneValidity(timezone);
    unixTime += timezone * 3600;
    time_t t = unixTime;
    tYear = year(t);
    tMonth = month(t);
    tDate = day(t);
    tHour = hour(t);
    tMinute = minute(t);
    tSecond = second(t);
    return true;
}

bool setRTCDateTime(RTCZero &rtc, int8_t timezone){
    if (!isWifiConnected()) return false;
    unsigned long unixTime = 0;
    if (getUnixTime(unixTime) != WIFI_GET_TIME_SUCCESS) return false;
    checkTimezoneValidity(timezone);
    unixTime += timezone * 3600;
    time_t t = unixTime;
    rtc.setTime(hour(t), minute(t), second(t));
    rtc.setDate(day(t), month(t), year(t));
    Serial.println(t);
    return true;
}

void getRTCDate(RTCZero &rtc, uint8_t &tDate, uint8_t &tMonth, uint16_t &tYear){
    tDate = rtc.getDay();
    tMonth = rtc.getMonth();
    tYear = rtc.getYear();
}

void getRTCTime(RTCZero &rtc, uint8_t &tHour, uint8_t &tMinute, uint8_t &tSecond){
    tHour = rtc.getHours();
    tMinute = rtc.getMinutes();
    tSecond = rtc.getSeconds();
}

void setAlarmTime(RTCZero &rtc, uint8_t tHour, uint8_t tMinute, uint8_t tSecond){
    checkTimeValidity(tHour, tMinute, tSecond);
    rtc.setAlarmTime(tHour, tMinute, tSecond);
}

void armAlarm(RTCZero &rtc, RTCZero::Alarm_Match alarmType, voidFuncPtr callback, bool enable){
    if (callback == nullptr) return;
    if (enable){
        rtc.enableAlarm(alarmType);
        rtc.attachInterrupt(callback);
    } else {
        rtc.detachInterrupt();
        rtc.disableAlarm();
    }
}