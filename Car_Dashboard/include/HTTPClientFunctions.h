#ifndef HTTP_CLIENT_FUNCTIONS
#define HTTP_CLIENT_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

enum HTTPReturnStatus{
    HTTP_HTTP_CLIENT_NULL,
    HTTP_WIFI_DISCONNECTED,
    HTTP_SERVER_NOT_CONNECTED,
    HTTP_CLIENT_ERROR,
    HTTP_SERVER_ERROR,
    HTTP_INTERNAL_LIB_ERROR,
    HTTP_COMMAND_SUCCESS
};

#define HTTPPort 80U
#define HTTPSPort 443U
#define HTTPTimeout 20000UL
HttpClient *initializeHTTPInstance(WiFiClient &wifiClientInstance, const char *URL, bool overrideWifiInstance = false);
HttpClient *initializeHTTPSInstance(WiFiClient &wifiClientInstance, const char *URL, bool overrideWifiInstance = false);
HTTPReturnStatus HTTPGet(HttpClient *client,const String request, String &payload);
HTTPReturnStatus HTTPPost(HttpClient *client, const String request, const String contentType, const String body, String &payload);
void closeHTTPInstance(HttpClient *client);

#endif