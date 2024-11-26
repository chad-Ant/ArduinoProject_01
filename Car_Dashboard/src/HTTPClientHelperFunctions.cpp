#include "../include/HTTPClientFunctions.h"
#include "../include/WiFiFunctions.h"

bool initializeHTTPInstance(WiFiClient &wifi, HTTPClient &http, const String URL){
    if (WiFi.status != WL_CONNECTED) return false;
    http = HttpClient(wifi,http,HTTPSPort);
    return true;
}