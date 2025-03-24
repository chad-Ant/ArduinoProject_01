#include "../include/HTTPClientFunctions.h"
#include "../include/WiFiFunctions.h"
#include "../include/TimerFunctions.h"

HttpClient *initializeHTTPInstance(WiFiClient &wifiClientInstance, const char *URL, bool overrideWifiInstance){
    if (wifiClientInstance.connected() && !overrideWifiInstance) return NULL;
    
    if (wifiClientInstance.connected()){
        wifiClientInstance.stop();
        wifiClientInstance.flush();
    }
    HttpClient *http = new HttpClient(wifiClientInstance, URL, HTTPPort);
    if (!http) return NULL;

    return http;
}

HttpClient *initializeHTTPSInstance(WiFiClient &wifiClientInstance, const char *URL, bool overrideWifiInstance){
    if (wifiClientInstance.connected() && !overrideWifiInstance) return NULL;
    
    if (wifiClientInstance.connected()){
        wifiClientInstance.stop();
        wifiClientInstance.flush();
    }
    HttpClient *https = new HttpClient(wifiClientInstance, URL, HTTPSPort);
    return https;
}

int HTTPGet(HttpClient *client,const String request, String &payload){
    if (!client) return -1;

    if (!client->connected()){
        unsigned long timeout = millis();
        while (!(client->connected() || isTimeout(HTTPTimeout,timeout))){
            client->get(request);
            delay(100);
        }
    }
    if (!client->connected()) return -1;    
    int responseCode = client->responseStatusCode();
    payload = client->responseBody();
    return responseCode;
}

int HTTPPost(HttpClient *client, const String request, const String contentType, const String body, String &payload){
    if (!client || !client->connected()) return -1;

    client->post(request,contentType,body);
    int responseCode = client->responseStatusCode();
    payload = client->responseBody();
    Serial.println(payload);
    return responseCode;
}

void closeHTTPInstance(HttpClient *client){
    client->stop();
    delete client;
    return;
}