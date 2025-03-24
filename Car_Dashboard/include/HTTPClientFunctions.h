#ifndef HTTP_CLIENT_FUNCTIONS
#define HTTP_CLIENT_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

#define HTTPPort 80U
#define HTTPSPort 443U
#define HTTPTimeout 20000UL
HttpClient *initializeHTTPInstance(WiFiClient &wifiClientInstance, const char *URL, bool overrideWifiInstance = false);
HttpClient *initializeHTTPSInstance(WiFiClient &wifiClientInstance, const char *URL, bool overrideWifiInstance = false);
int HTTPGet(HttpClient *client,const String request, String &payload);
int HTTPPost(HttpClient *client, const String request, const String contentType, const String body, String &payload);
void closeHTTPInstance(HttpClient *client);

#endif