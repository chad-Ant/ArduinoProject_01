#include "../include/WiFiFunctions.h"

bool initializeWifi(){
    if (WiFi.status() == WL_NO_SHIELD) return false;

    unsigned long beginTime = millis();

    WiFi.begin(WIFI_SSID,WIFI_PASS);
    while (millis() - beginTime <= WIFI_TIMEOUT_MSEC || WiFi.status() != WL_CONNECTED);
    if (WiFi.status() == WL_CONNECTED) return true;
    return false;
}
