#ifndef WIFI_FUNCTIONS
#define WIFI_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

#define WIFI_MAX_RETRY 3U
#define WIFI_WAIT_MSEC 5000UL

enum WiFiReturnStatus{
    WIFI_SUCCESS,
    WIFI_FAILED_NO_SHIELD,
    WIFI_FAILED_NO_CONNECTION,
    CFG_STATIC_IP_SUCCESS,
    CFG_STATIC_IP_FAILED_NO_SHIELD,
    CFG_STATIC_IP_FAILED_EMPTY_IP,
    CFG_STATIC_IP_FAILED_EMPTY_DNS,
    CFG_STATIC_IP_FAILED_IP_MISMATCH,
    GET_TIME_SUCCESS,
    GET_TIME_FAILED_NO_CONNECTION,
    GET_TIME_FAILED_ZERO_TIME,
    GET_IP_SUCCESS,
    GET_IP_FAILED_NO_CONNECTION,
    GET_IP_FAILED_EMPTY_IP,
    CLOSE_SUCCESS,
    CLOSE_FAILED
};

WiFiReturnStatus initializeWifi();
WiFiReturnStatus configStaticIP(const IPAddress ip, const IPAddress dns);
bool isWifiConnected();
WiFiReturnStatus getUnixTime(unsigned long &unixTime);
WiFiReturnStatus getLocalIP(IPAddress &localIP);
WiFiReturnStatus closeWifi();
void turnoffWifi();

#endif