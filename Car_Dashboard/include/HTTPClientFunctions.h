#ifndef HTTP_CLIENT_FUNCTIONS
#define HTTP_CLIENT_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

constexpr uint16_t HTTPPort = 80;
constexpr uint16_t HTTPSPort = 443;

HttpClient *initializeHTTPInstance(WiFiClient &wifiClientInstance, const char *URL, bool overrideWifiInstance);
HttpClient *initializeHTTPSInstance(WiFiClient &wifiClientInstance, const char *URL, bool overrideWifiInstance);
int HTTPGet(HttpClient *client,const String request, String &payload);
int HTTPPost(HttpClient *client, const String request, const String contentType, const String body, String &payload);
void closeHTTPInstance(HttpClient *client);

#endif