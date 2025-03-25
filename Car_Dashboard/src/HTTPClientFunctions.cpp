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

    http->setHttpResponseTimeout(HTTPTimeout);
    return http;
}

HttpClient *initializeHTTPSInstance(WiFiClient &wifiClientInstance, const char *URL, bool overrideWifiInstance){
    if (wifiClientInstance.connected() && !overrideWifiInstance) return NULL;
    
    if (wifiClientInstance.connected()){
        wifiClientInstance.stop();
        wifiClientInstance.flush();
    }
    HttpClient *https = new HttpClient(wifiClientInstance, URL, HTTPSPort);
    if (!https) return NULL;

    https->setHttpResponseTimeout(HTTPTimeout);
    return https;
}

long HTTPGet(HttpClient *client,const String request, String &payload){
    if (!client) return -1;
    client->get(request);
    if (!client->connected()) return -1;    
    int responseCode = client->responseStatusCode();
    if (responseCode >= 400) return -1;
    if (responseCode < 0) return (long)responseCode;

    payload = client->responseBody();
    return client->contentLength();
}

long HTTPPost(HttpClient *client, const String request, const String contentType, const String body, String &payload){
    if (!client) return -1;
    client->post(request,contentType,body);
    if (!client->connected()) return -1;
    int responseCode = client->responseStatusCode();
    if (responseCode >= 400) return -1;
    if (responseCode < 0) return (long)responseCode;

    payload = client->responseBody();
    return client->contentLength();
}

void closeHTTPInstance(HttpClient *client){
    client->stop();
    delete client;
    return;
}