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

bool configStaticIP(const IPAddress ip){
    if (WiFi.status() == WL_NO_SHIELD || ip == INADDR_NONE) return false;
    WiFi.config(ip);
    return WiFi.localIP() == ip;
}

bool configDNS(const IPAddress dns1, const IPAddress dns2){
    if (WiFi.status() == WL_NO_SHIELD || (dns1 == INADDR_NONE && dns2 == INADDR_NONE)) return false;
    if (dns1 != INADDR_NONE && dns2 == INADDR_NONE)
        WiFi.setDNS(dns1);
    else if (dns1 == INADDR_NONE && dns2 != INADDR_NONE) 
        WiFi.setDNS(dns2);
    else
        WiFi.setDNS(dns1,dns2);
    return true;
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