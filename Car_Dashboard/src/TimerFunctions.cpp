#include "../include/TimerFunctions.h"
#include "../include/WiFiFunctions.h"

TimerReturnStatus initializeRTC(RTCZero &rtc){
    rtc.begin();
    if (rtc.isConfigured()) return TIMER_RTC_CFG_SUCCESS;
    return TIMER_RTC_CFG_FAILED;
}

void checkTimeValidity(uint8_t &tHour, uint8_t &tMinute, uint8_t &tSecond){
    tHour = tHour > 23 ? 23 : tHour;
    tMinute = tMinute > 59 ? 59 : tMinute;
    tSecond = tSecond > 59 ? 59 : tSecond;
}

void checkTimezoneValidity(int8_t &timezone){
    timezone = timezone < -12 ? -12 : (timezone > 12 ? 12 : timezone);
}

TimerReturnStatus getNTPDate(uint8_t &tDate, uint8_t &tMonth, uint16_t &tYear, int8_t timezone){ //areas with fractional timezone offsets not supported
    if (!isWifiConnected()) return TIMER_NTP_FAILED_NO_CONNECTION;

    unsigned long unixTime = 0;
    if (!getUnixTime(unixTime)) return TIMER_NTP_FAILED_ZERO_TIME;

    checkTimezoneValidity(timezone);
    unixTime += timezone * 3600;
    time_t t = unixTime;
    tYear = year(t);
    tMonth = month(t);
    tDate = day(t);
    return TIMER_NTP_SUCCESS;
}

TimerReturnStatus getNTPTime(uint8_t &tHour, uint8_t &tMinute, uint8_t &tSecond, int8_t timezone){
    if (!isWifiConnected()) return TIMER_NTP_FAILED_NO_CONNECTION;

    unsigned long unixTime = 0;
    if (!getUnixTime(unixTime)) return TIMER_NTP_FAILED_ZERO_TIME;

    checkTimezoneValidity(timezone);
    unixTime += timezone * 3600;
    time_t t = unixTime;
    tHour = hour(t);
    tMinute = minute(t);
    tSecond = second(t);
    return TIMER_NTP_SUCCESS;
}

TimerReturnStatus getNTPDateTime(uint8_t &tHour, uint8_t &tMinute, uint8_t &tSecond, uint8_t &tDate, uint8_t &tMonth, uint16_t &tYear, int8_t timezone){
    if (!isWifiConnected()) return TIMER_NTP_FAILED_NO_CONNECTION;

    unsigned long unixTime = 0;
    if (!getUnixTime(unixTime)) return TIMER_NTP_FAILED_ZERO_TIME;

    checkTimezoneValidity(timezone);
    unixTime += timezone * 3600;
    time_t t = unixTime;
    tYear = year(t);
    tMonth = month(t);
    tDate = day(t);
    tHour = hour(t);
    tMinute = minute(t);
    tSecond = second(t);
    return TIMER_NTP_SUCCESS;
}

TimerReturnStatus setRTCDateTime(RTCZero &rtc, int8_t timezone){
    if (!isWifiConnected()) return TIMER_RTC_SET_FAILED_NO_CONNECTION;
    if (!rtc.isConfigured()) return TIMER_RTC_CFG_FAILED;

    unsigned long unixTime = 0;
    if (getUnixTime(unixTime) != WIFI_GET_TIME_SUCCESS) return TIMER_NTP_FAILED_ZERO_TIME;

    checkTimezoneValidity(timezone);
    unixTime += timezone * 3600;
    time_t t = unixTime;
    rtc.setTime((int8_t)(hour(t)), (int8_t)(minute(t)), (int8_t)(second(t)));
    rtc.setDate((int8_t)(day(t)), (int8_t)(month(t)), (int8_t)(year(t) - 2000));
    return TIMER_RTC_SET_SUCCESS;
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

TimerReturnStatus setAlarmTime(RTCZero &rtc, uint8_t tHour, uint8_t tMinute, uint8_t tSecond){
    if (!rtc.isConfigured()) return TIMER_RTC_CFG_FAILED;
    checkTimeValidity(tHour, tMinute, tSecond);
    rtc.setAlarmTime(tHour, tMinute, tSecond);
    if (rtc.getAlarmSeconds() == tSecond && rtc.getAlarmMinutes() == tMinute && rtc.getAlarmHours() == tHour) return TIMER_RTC_SET_ALARM_SUCCESS;
    return TIMER_RTC_SET_ALARM_FAILED;
}

TimerReturnStatus armAlarm(RTCZero &rtc, RTCZero::Alarm_Match alarmType, voidFuncPtr callback, bool enable){
    if (!rtc.isConfigured()) return TIMER_RTC_CFG_FAILED;
    if (callback == nullptr) return TIMER_RTC_ARM_ALARM_FAILED_VOID_CALLBACK;
    
    if (enable){
        rtc.enableAlarm(alarmType);
        rtc.attachInterrupt(callback);
        return TIMER_RTC_ARM_ALARM_SUCCESS;
    } else {
        rtc.detachInterrupt();
        rtc.disableAlarm();
        return TIMER_RTC_DISARM_ALARM_SUCCESS;
    }
}