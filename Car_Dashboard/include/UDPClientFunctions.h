#ifndef UDP_CLIENT_FUNCTIONS
#define UDP_CLIENT_FUNCTIONS 1

#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>   // WiFi101 radio stack (MKR 1000 WiFi)
#include <WiFiUdp.h>   // WiFiUDP
#include "../config/DataDictionary.h"

/// Local UDP port used for all outgoing and incoming UDP datagrams.
#define UDPPort 2390U

/**
 * @brief Brings up WiFi (if needed) and opens a UDP socket on @c UDPPort.
 *
 * Calls @c initializeWifi() first; if the radio cannot associate, no socket is
 * created and @c nullptr is returned.
 *
 * @return Pointer to a new @c WiFiUDP bound to @c UDPPort, or @c nullptr if
 *         WiFi failed to connect or allocation failed.
 * @note Caller owns the returned instance — free it with @c closeUDPInstance().
 */
WiFiUDP *initializeUDPInstance();

/**
 * @brief Stops the UDP socket and frees the instance.
 *
 * Safe to call with @c nullptr.
 *
 * @param[in] client Instance to destroy; must not be used after this call.
 */
void closeUDPInstance(WiFiUDP *client);

#endif
