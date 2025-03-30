#ifndef HTTP_CLIENT_FUNCTIONS
#define HTTP_CLIENT_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

enum class HTTPReturnStatus{
    OK = 0,
    FAILED_CLIENT_NULL = -1,
    NOK_WIFI_DISCONNECTED = -2,
    NOK_NOT_CONNECTED = -3,
    NOK_CLIENT_ERROR = -4,
    NOK_SERVER_ERROR = -5,
    NOK_INTERNAL_ERROR = -6
};

#define HTTPPort 80U
#define HTTPSPort 443U
#define HTTPTimeout 20000UL
#define URL_BUFFER_SIZE 256U

HttpClient *initializeHTTPInstance(WiFiClient &wifiClientInstance, const char *URL, bool overrideWifiInstance = false);
HttpClient *initializeHTTPSInstance(WiFiClient &wifiClientInstance, const char *URL, bool overrideWifiInstance = false);
HTTPReturnStatus HTTPGet(HttpClient *client,const String request, String &payload);
HTTPReturnStatus HTTPPost(HttpClient *client, const String request, const String contentType, const String body, String &payload);
void closeHTTPInstance(HttpClient *client);

#endif