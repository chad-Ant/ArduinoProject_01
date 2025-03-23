#include "../include/UDPClientFunctions.h"
#include "../include/WiFiFunctions.h"

WiFiUDP *initializeUDPInstance(){
    if (!initializeWifi())
        return nullptr;
    WiFiUDP udp;
    udp.begin(UDPPort);
    return &udp;
}

String HTTPGet(WiFiUDP *client,const IPAddress address, const String invalidString)
{
    client->get(request);

    int responseCode = client->responseStatusCode();
    String payload = client->responseBody();

    if (responseCode == 200) return payload;
    return invalidString;
}