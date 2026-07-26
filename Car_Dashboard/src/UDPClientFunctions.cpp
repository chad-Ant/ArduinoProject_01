#include "../include/UDPClientFunctions.h"
#include "../include/WiFiFunctions.h"

// -Wdelete-non-virtual-dtor is a false positive for this file: WiFiUDP is polymorphic
// with a non-virtual dtor, but every instance here is allocated and deleted through the
// *exact* same type (WiFiUDP*), never through a base pointer, so no UB is possible.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"

WiFiUDP *initializeUDPInstance(){
    if (initializeWifi() != WiFiReturnStatus::OK)
        return nullptr;

    WiFiUDP *udp = new WiFiUDP();
    if (!udp) return nullptr;

    if (!udp->begin(UDPPort)){   // 0 = no socket available; don't leak the instance
        delete udp;
        return nullptr;
    }
    return udp;
}

void closeUDPInstance(WiFiUDP *client){
    if (!client) return;
    client->stop();
    delete client;
}

#pragma GCC diagnostic pop

/*
 * UNFINISHED STUB — intentionally left un-declared in the header so nothing can
 * call it yet. It is named "HTTPGet" but takes a WiFiUDP, and its body is entirely
 * commented out (it just echoes back invalidString). Decide what this should
 * actually be (e.g. a UDP request/response helper) before wiring it up.
 *
 * String HTTPGet(WiFiUDP *client, const IPAddress address, const String invalidString)
 * {
 *     int responseCode = client->responseStatusCode();
 *     String payload = client->responseBody();
 *     if (responseCode == 200) return payload;
 *     return invalidString;
 * }
 */
