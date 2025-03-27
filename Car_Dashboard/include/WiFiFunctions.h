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
    WIFI_CFG_STC_IP_SUCCESS,
    WIFI_CFG_STC_IP_FAILED_NO_SHIELD,
    WIFI_CFG_STC_IP_FAILED_EMPTY_IP,
    WIFI_CFG_STC_IP_FAILED_EMPTY_DNS,
    WIFI_CFG_STC_IP_FAILED_IP_MISMATCH,
    WIFI_GET_TIME_SUCCESS,
    WIFI_GET_TIME_FAILED_NO_CONNECTION,
    WIFI_GET_TIME_FAILED_ZERO_TIME,
    WIFI_GET_IP_SUCCESS,
    WIFI_GET_IP_FAILED_NO_CONNECTION,
    WIFI_GET_IP_FAILED_EMPTY_IP,
    WIFI_CLOSE_SUCCESS,
    WIFI_CLOSE_FAILED
};

WiFiReturnStatus initializeWifi();
WiFiReturnStatus configStaticIP(const IPAddress ip, const IPAddress dns);
bool isWifiConnected();
WiFiReturnStatus getUnixTime(unsigned long &unixTime);
WiFiReturnStatus getLocalIP(IPAddress &localIP);
WiFiReturnStatus closeWifi();
void turnoffWifi();

#endif