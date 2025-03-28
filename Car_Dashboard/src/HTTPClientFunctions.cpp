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

HTTPReturnStatus HTTPGet(HttpClient *client,const String request, String &payload){
    if (!client) return HTTP_HTTP_CLIENT_NULL;
    if (!isWifiConnected()) return HTTP_WIFI_DISCONNECTED;
    
    client->get(request);
    if (!client->connected()) return HTTP_SERVER_NOT_CONNECTED;    
    int responseCode = client->responseStatusCode();
    if (responseCode >= 400 && responseCode < 500) return HTTP_CLIENT_ERROR;
    if (responseCode >= 500) return HTTP_SERVER_ERROR;
    if (responseCode < 0) return HTTP_INTERNAL_LIB_ERROR;

    payload = client->responseBody();
    return HTTP_COMMAND_SUCCESS;
}

HTTPReturnStatus HTTPPost(HttpClient *client, const String request, const String contentType, const String body, String &payload){
    if (!client) return HTTP_HTTP_CLIENT_NULL;
    if (!isWifiConnected()) return HTTP_WIFI_DISCONNECTED;
    
    client->post(request,contentType,body);
    if (!client->connected()) return HTTP_SERVER_NOT_CONNECTED;
    int responseCode = client->responseStatusCode();
    if (responseCode >= 400 && responseCode < 500) return HTTP_CLIENT_ERROR;
    if (responseCode >= 500) return HTTP_SERVER_ERROR;
    if (responseCode < 0) return HTTP_INTERNAL_LIB_ERROR;

    payload = client->responseBody();
    return HTTP_COMMAND_SUCCESS;
}

void closeHTTPInstance(HttpClient *client){
    client->stop();
    delete client;
    return;
}