#include "../include/WiFiFunctions.h"
#include "../include/TimerFunctions.h"

WiFiReturnStatus initializeWifi(){
    if (WiFi.status() == WL_NO_SHIELD) return WIFI_FAILED_NO_SHIELD;

    unsigned long startTime = 0;
    for (int i = 0; i < WIFI_MAX_RETRY; i++){
        WiFi.begin(WIFI_SSID,WIFI_PASS);
        startTime = millis();
        while (!isTimeout(WIFI_WAIT_MSEC,startTime));
        if (WiFi.status() == WL_CONNECTED) return WIFI_SUCCESS;
    }
    return WIFI_FAILED_NO_CONNECTION;
}

WiFiReturnStatus configStaticIP(const IPAddress ip, const IPAddress dns){
    if (WiFi.status() == WL_NO_SHIELD) CFG_STATIC_IP_FAILED_NO_SHIELD;
    if (ip == INADDR_NONE) return CFG_STATIC_IP_FAILED_EMPTY_IP;
    if (dns == INADDR_NONE) return CFG_STATIC_IP_FAILED_EMPTY_DNS;

    WiFi.config(ip, dns);
    if (WiFi.localIP() == ip) return CFG_STATIC_IP_SUCCESS;
    else return CFG_STATIC_IP_FAILED_IP_MISMATCH;
}

bool isWifiConnected(){
    return WiFi.status() == WL_CONNECTED;
}

WiFiReturnStatus getUnixTime(unsigned long &unixTime){
    if (WiFi.status() != WL_CONNECTED) return GET_TIME_FAILED_NO_CONNECTION;
    unixTime = WiFi.getTime();
    if (unixTime != 0) return GET_TIME_SUCCESS;
    else return GET_TIME_FAILED_ZERO_TIME;
}

WiFiReturnStatus getLocalIP(IPAddress &localIP){
    if (WiFi.status() != WL_CONNECTED) return GET_IP_FAILED_NO_CONNECTION;
    localIP = WiFi.localIP();
    if (localIP != INADDR_NONE) return GET_IP_SUCCESS;
    else return GET_IP_FAILED_EMPTY_IP;
}

WiFiReturnStatus closeWifi(){
    WiFi.disconnect();
    if (WiFi.status() != WL_CONNECTED) return CLOSE_SUCCESS;
    else return CLOSE_FAILED;

}

void turnoffWifi(){
    WiFi.disconnect();
    WiFi.end();
}