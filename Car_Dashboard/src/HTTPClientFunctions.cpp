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
    if (!client) return HTTPReturnStatus::FAILED_CLIENT_NULL;
    if (!isWifiConnected()) return HTTPReturnStatus::NOK_WIFI_DISCONNECTED;
    
    client->get(request);
    if (!client->connected()) return HTTPReturnStatus::NOK_NOT_CONNECTED;    
    int responseCode = client->responseStatusCode();
    if (responseCode >= 400 && responseCode < 500) return HTTPReturnStatus::NOK_CLIENT_ERROR;
    if (responseCode >= 500) return HTTPReturnStatus::NOK_SERVER_ERROR;
    if (responseCode < 0) return HTTPReturnStatus::NOK_INTERNAL_ERROR;

    payload = client->responseBody();
    return HTTPReturnStatus::OK;
}

HTTPReturnStatus HTTPPost(HttpClient *client, const String request, const String contentType, const String body, String &payload){
    if (!client) return HTTPReturnStatus::FAILED_CLIENT_NULL;
    if (!isWifiConnected()) return HTTPReturnStatus::NOK_WIFI_DISCONNECTED;
    
    client->post(request,contentType,body);
    if (!client->connected()) return HTTPReturnStatus::NOK_NOT_CONNECTED;
    int responseCode = client->responseStatusCode();
    if (responseCode >= 400 && responseCode < 500) return HTTPReturnStatus::NOK_CLIENT_ERROR;
    if (responseCode >= 500) return HTTPReturnStatus::NOK_SERVER_ERROR;
    if (responseCode < 0) return HTTPReturnStatus::NOK_INTERNAL_ERROR;

    payload = client->responseBody();
    return HTTPReturnStatus::OK;
}

void closeHTTPInstance(HttpClient *client){
    // Guard: initializeHTTPInstance()/initializeHTTPSInstance() return NULL on failure,
    // so a caller cleaning up after an error would otherwise dereference NULL here.
    if (!client) return;
    client->stop();
    // -Wdelete-non-virtual-dtor is a false positive here: HttpClient is polymorphic with a
    // non-virtual dtor, but we allocate and delete through the *exact* same type
    // (HttpClient*), never through a base pointer, so no UB is possible.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
    delete client;
#pragma GCC diagnostic pop
}