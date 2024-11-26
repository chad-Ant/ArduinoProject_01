#include "../include/HTTPClientFunctions.h"
#include "../include/WiFiFunctions.h"

HttpClient* initializeHTTPInstance(const String URL){
    if (WiFi.status != WL_CONNECTED) return nullptr;
    WiFiClient wifi;
    HttpClient http = HttpClient(wifi,http,HTTPPort);
    return &http;
}