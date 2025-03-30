#include "../include/WiFiFunctions.h"
#include "../include/TimerFunctions.h"

WiFiReturnStatus initializeWifi(){
    if (WiFi.status() == WL_NO_SHIELD) return WiFiReturnStatus::NOK_NO_SHIELD;

    unsigned long startTime = 0;
    char *SSIDs[] = {WIFI_SSID,WIFI_SSID_BACKUP_1,WIFI_SSID_BACKUP_2,WIFI_PASS,WIFI_PASS_BACKUP_1,WIFI_PASS_BACKUP_2};
    for (int i = 0; i <= 2; i++){
        WiFi.begin(SSIDs[i],SSIDs[i+3]);
        startTime = millis();
        while (!isTimeout(WIFI_WAIT_MSEC,startTime));
        if (WiFi.status() == WL_CONNECTED) return WiFiReturnStatus::OK;
    }
    return WiFiReturnStatus::NOK_NO_CONNECTION;
}

WiFiReturnStatus configStaticIP(const IPAddress ip, const IPAddress dns){
    if (WiFi.status() == WL_NO_SHIELD) WiFiReturnStatus::NOK_NO_SHIELD;
    if (ip == INADDR_NONE) return WiFiReturnStatus::NOK_EMPTY_IP;
    if (dns == INADDR_NONE) return WiFiReturnStatus::NOK_EMPTY_DNS;

    WiFi.config(ip, dns);
    if (WiFi.localIP() == ip) return WiFiReturnStatus::OK;
    else return WiFiReturnStatus::NOK_IP_MISMATCH;
}

bool isWifiConnected(){
    return WiFi.status() == WL_CONNECTED;
}

WiFiReturnStatus getUnixTime(unsigned long &unixTime){
    if (WiFi.status() != WL_CONNECTED) return WiFiReturnStatus::NOK_NO_CONNECTION;
    unixTime = WiFi.getTime(); //static IP will cause this to fail, suggest using dynamic IP
    if (unixTime != 0) return WiFiReturnStatus::OK;
    else return WiFiReturnStatus::NOK_ZERO_TIME;
}

WiFiReturnStatus getLocalIP(IPAddress &localIP){
    if (WiFi.status() != WL_CONNECTED) return WiFiReturnStatus::NOK_NO_CONNECTION;
    localIP = WiFi.localIP();
    if (localIP != INADDR_NONE) return WiFiReturnStatus::OK;
    else return WiFiReturnStatus::NOK_EMPTY_IP;
}

WiFiReturnStatus closeWifi(){
    WiFi.disconnect();
    if (WiFi.status() != WL_CONNECTED) return WiFiReturnStatus::OK;
    else return WiFiReturnStatus::NOK_CLOSE_FAILED;

}

void turnoffWifi(){
    WiFi.disconnect();
    WiFi.end();
}