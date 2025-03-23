#ifndef WIFI_FUNCTIONS
#define WIFI_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

bool initializeWifi();
bool configStaticIP(const IPAddress ip, const IPAddress dns);
bool isWifiConnected();
bool getUnixTime(unsigned long &unixTime);
bool getLocalIP(IPAddress &localIP);
bool closeWifi();
void turnoffWifi();

#endif