#ifndef WIFI_FUNCTIONS
#define WIFI_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

//WiFi uses SPI1

#define WIFI_MAX_RETRY 3U
#define WIFI_WAIT_MSEC 5000UL

enum class WiFiReturnStatus{
    OK = 0,
    NOK_NO_SHIELD = -1,
    NOK_NO_CONNECTION = -2,
    NOK_EMPTY_IP = -3,
    NOK_EMPTY_DNS = -4,
    NOK_IP_MISMATCH = -5,
    NOK_ZERO_TIME = -6,
    NOK_CLOSE_FAILED = -7
};

WiFiReturnStatus initializeWifi();
WiFiReturnStatus configStaticIP(const IPAddress ip, const IPAddress dns);
bool isWifiConnected();
WiFiReturnStatus getUnixTime(unsigned long &unixTime);
WiFiReturnStatus getLocalIP(IPAddress &localIP);
WiFiReturnStatus closeWifi();
void turnoffWifi();

#endif