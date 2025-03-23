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

bool configStaticIP(const IPAddress ip, const IPAddress dns){
    if (WiFi.status() == WL_NO_SHIELD || ip == INADDR_NONE || dns == INADDR_NONE) return false;
    WiFi.config(ip, dns);
    return WiFi.localIP() == ip;
}

bool isWifiConnected(){
    return WiFi.status() == WL_CONNECTED;
}

bool getUnixTime(unsigned long &unixTime){
    if (WiFi.status() != WL_CONNECTED) return false;
    unixTime = WiFi.getTime();
    return unixTime != 0;
}

bool getLocalIP(IPAddress &localIP){
    if (WiFi.status() != WL_CONNECTED) return false;
    localIP = WiFi.localIP();
    return localIP != INADDR_NONE;
}

bool closeWifi(){
    WiFi.disconnect();
    return WiFi.status() != WL_CONNECTED;
}

void turnoffWifi(){
    WiFi.disconnect();
    WiFi.end();
}