#include <TimeLib.h>   // time_t, year/month/day/hour/minute/second for the NTP helpers

#include "../include/WiFiFunctions.h"
#include "../include/TimerFunctions.h"

WiFiReturnStatus initializeWifi(){
    if (WiFi.status() == WL_NO_SHIELD) return WiFiReturnStatus::NOK_NO_SHIELD;

    // const: these are string literals from DataDictionary.h (plain char* trips -Wwrite-strings).
    const char *SSIDs[] = {WIFI_SSID,WIFI_SSID_BACKUP_1,WIFI_SSID_BACKUP_2,WIFI_PASS,WIFI_PASS_BACKUP_1,WIFI_PASS_BACKUP_2};
    for (int i = 0; i <= 2; i++){
        WiFi.begin(SSIDs[i],SSIDs[i+3]);
        // Poll for association and return as soon as it succeeds. The old code always
        // burned the full WIFI_WAIT_MSEC per SSID even after connecting.
        unsigned long startTime = millis();
        while (!isTimeout(WIFI_WAIT_MSEC,startTime)){
            if (WiFi.status() == WL_CONNECTED) return WiFiReturnStatus::OK;
            delay(100);
        }
        if (WiFi.status() == WL_CONNECTED) return WiFiReturnStatus::OK;
    }
    return WiFiReturnStatus::NOK_NO_CONNECTION;
}

WiFiReturnStatus configStaticIP(const IPAddress ip, const IPAddress dns){
    if (WiFi.status() == WL_NO_SHIELD) return WiFiReturnStatus::NOK_NO_SHIELD;
    if (ip == INADDR_NONE) return WiFiReturnStatus::NOK_EMPTY_IP;
    if (dns == INADDR_NONE) return WiFiReturnStatus::NOK_EMPTY_DNS;

    WiFi.config(ip, dns);
    if (WiFi.localIP() == ip) return WiFiReturnStatus::OK;
    else return WiFiReturnStatus::NOK_IP_MISMATCH;
}

bool isWifiConnected(){
    return WiFi.status() == WL_CONNECTED;
}

WiFiReturnStatus getUnixTime(unsigned long &unixTime){
    if (WiFi.status() != WL_CONNECTED) return WiFiReturnStatus::NOK_NO_CONNECTION;
    unixTime = WiFi.getTime(); //static IP will cause this to fail, suggest using dynamic IP
    if (unixTime != 0) return WiFiReturnStatus::OK;
    else return WiFiReturnStatus::NOK_ZERO_TIME;
}

WiFiReturnStatus getLocalIP(IPAddress &localIP){
    if (WiFi.status() != WL_CONNECTED) return WiFiReturnStatus::NOK_NO_CONNECTION;
    localIP = WiFi.localIP();
    if (localIP != INADDR_NONE) return WiFiReturnStatus::OK;
    else return WiFiReturnStatus::NOK_EMPTY_IP;
}

static void clampTimezone(int8_t &tz){
    tz = tz < -12 ? -12 : (tz > 12 ? 12 : tz);
}

WiFiReturnStatus getNTPDate(uint8_t &tDate, uint8_t &tMonth, uint16_t &tYear, int8_t timezone){
    unsigned long unixTime = 0;
    WiFiReturnStatus s = getUnixTime(unixTime);
    if (s != WiFiReturnStatus::OK) return s;

    clampTimezone(timezone);
    time_t t = (time_t)(unixTime + (long)timezone * 3600L);
    tYear  = year(t);
    tMonth = month(t);
    tDate  = day(t);
    return WiFiReturnStatus::OK;
}

WiFiReturnStatus getNTPTime(uint8_t &tHour, uint8_t &tMinute, uint8_t &tSecond, int8_t timezone){
    unsigned long unixTime = 0;
    WiFiReturnStatus s = getUnixTime(unixTime);
    if (s != WiFiReturnStatus::OK) return s;

    clampTimezone(timezone);
    time_t t = (time_t)(unixTime + (long)timezone * 3600L);
    tHour   = hour(t);
    tMinute = minute(t);
    tSecond = second(t);
    return WiFiReturnStatus::OK;
}

WiFiReturnStatus getNTPDateTime(uint8_t &tHour, uint8_t &tMinute, uint8_t &tSecond,
                                uint8_t &tDate, uint8_t &tMonth, uint16_t &tYear,
                                int8_t timezone){
    unsigned long unixTime = 0;
    WiFiReturnStatus s = getUnixTime(unixTime);
    if (s != WiFiReturnStatus::OK) return s;

    clampTimezone(timezone);
    time_t t = (time_t)(unixTime + (long)timezone * 3600L);
    tYear   = year(t);
    tMonth  = month(t);
    tDate   = day(t);
    tHour   = hour(t);
    tMinute = minute(t);
    tSecond = second(t);
    return WiFiReturnStatus::OK;
}

WiFiReturnStatus setRTCDateTime(RTCZero &rtc, int8_t timezone){
    if (!rtc.isConfigured()) return WiFiReturnStatus::NOK_RTC_FAILED;

    uint8_t  tHour = 0, tMinute = 0, tSecond = 0, tDate = 0, tMonth = 0;
    uint16_t tYear = 0;
    WiFiReturnStatus s = getNTPDateTime(tHour, tMinute, tSecond, tDate, tMonth, tYear, timezone);
    if (s != WiFiReturnStatus::OK) return s;

    rtc.setTime(tHour, tMinute, tSecond);
    rtc.setDate(tDate, tMonth, (uint8_t)(tYear - 2000u));
    return WiFiReturnStatus::OK;
}

WiFiReturnStatus closeWifi(){
    WiFi.disconnect();
    if (WiFi.status() != WL_CONNECTED) return WiFiReturnStatus::OK;
    else return WiFiReturnStatus::NOK_CLOSE_FAILED;

}

void turnoffWifi(){
    WiFi.disconnect();
    WiFi.end();
}