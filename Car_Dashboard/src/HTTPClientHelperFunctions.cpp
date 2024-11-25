#include "../include/HTTPClientFunctions.h"
#include "../include/WiFiFunctions.h"

HTTPClient* initializeHTTPInstance(const String URL){
    if (WiFi.status != WL_CONNECTED) return nullptr;
    WiFiClient client;
    HTTPClient http(client,URL,);
    
    return *http;
}