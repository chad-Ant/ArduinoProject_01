#ifndef WIFI_FUNCTIONS
#define WIFI_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

bool initializeWifi();
bool configStaticIP(const IPAddress ip);
bool configDNS(const IPAddress dns1, const IPAddress dns2 = INADDR_NONE);
bool isWifiConnected();
bool getUnixTime(unsigned long &unixTime);
bool getLocalIP(IPAddress &localIP);
bool closeWifi();
void turnoffWifi();

#endif