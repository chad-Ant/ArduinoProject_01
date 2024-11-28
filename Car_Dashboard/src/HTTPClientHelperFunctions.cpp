#include "../include/HTTPClientFunctions.h"
#include "../include/WiFiFunctions.h"

HttpClient *initializeHTTPInstance(const char *URL)
{
    if (WiFi.status() != WL_CONNECTED)
        return nullptr;
    WiFiClient wifi;
    HttpClient http = HttpClient(wifi, URL, HTTPPort);
    return &http;
}

String HTTPGet(HttpClient *client,const String request, const String invalidString)
{
    client->get(request);

    int responseCode = client->responseStatusCode();
    String payload = client->responseBody();

    if (responseCode == 200)
    {
        return payload;
    }
    return invalidString;
}