#ifndef HTTP_CLIENT_FUNCTIONS
#define HTTP_CLIENT_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

constexpr uint16_t HTTPPort = 80;
constexpr uint16_t HTTPSPort = 443;

HttpClient* initializeHTTPInstance(const char* URL);
String HTTPGet(HttpClient *client,const String request, const String invalidString);

#endif