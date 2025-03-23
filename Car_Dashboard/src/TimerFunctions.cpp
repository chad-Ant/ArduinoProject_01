#include "../include/TimerFunctions.h"
#include "../include/WiFiFunctions.h"
#include "../include/QuickMathFunctions.h"

bool getDate(uint8_t &_date, uint8_t &_month, uint16_t &_year, const uint8_t timezone){ //areas with fractional timezone offsets not supported
    if (!isWifiConnected()) return false;
    unsigned long unixTime = getUnixTime();
    if (unixTime == 0) return false;
    unixTime += timezone * 3600;
    
    time_t t = unixTime;

    _year = year(t);
    _month = month(t);
    _date = day(t);
    return true;
}

bool getTime(uint8_t &_hour, uint8_t &_minute, uint8_t &_second, const uint8_t timezone){
    if (!isWifiConnected()) return false;
    unsigned long unixTime = getUnixTime();
    if (unixTime == 0) return false;
    unixTime += timezone * 3600;
    
    time_t t = unixTime;

    _hour = hour(t);
    _minute = minute(t);
    _second = second(t);
    return true;
}