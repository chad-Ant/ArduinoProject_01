#include "../include/WiFiFunctions.h"
#include "../include/TimerFunctions.h"

bool initializeWifi(){
    if (WiFi.status() == WL_NO_SHIELD) return false;

    unsigned long startTime = millis();

    WiFi.begin(WIFI_SSID,WIFI_PASS);
    while (!isTimeout(WIFI_TIMEOUT_MSEC, startTime) || WiFi.status() != WL_CONNECTED);
    if (WiFi.status() == WL_CONNECTED) return true;
    return false;
}

bool isWifiConnected(){
    return WiFi.status() == WL_CONNECTED;
}

bool getUnixTime(unsigned long &unixTime){
    if WiFi.status() != WL_CONNECTED return false;
    unixTime = WiFi.getTime();
    return unixTime != 0;
}

bool closeWifi(){
    WiFi.disconnect();
    return WiFi.status() != WL_CONNECTED;
}