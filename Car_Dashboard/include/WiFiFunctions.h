#ifndef WIFI_FUNCTIONS
#define WIFI_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

#define WIFI_MAX_RETRY 3U
#define WIFI_WAIT_MSEC 5000UL

bool initializeWifi();
bool configStaticIP(const IPAddress ip, const IPAddress dns);
bool isWifiConnected();
bool getUnixTime(unsigned long &unixTime);
bool getLocalIP(IPAddress &localIP);
bool closeWifi();
void turnoffWifi();

#endif