#ifndef HTTP_CLIENT_FUNCTIONS
#define HTTP_CLIENT_FUNCTIONS 1

#include <Arduino.h>            // String
#include <SPI.h>
#include <WiFi101.h>            // WiFiClient
#include <ArduinoHttpClient.h>  // HttpClient
#include "../config/DataDictionary.h"

/** Return codes used by HTTP client functions. */
enum class HTTPReturnStatus{
    OK = 0,                  ///< Request succeeded (2xx response).
    FAILED_CLIENT_NULL = -1, ///< Passed @c HttpClient pointer is NULL.
    NOK_WIFI_DISCONNECTED = -2, ///< WiFi is not connected.
    NOK_NOT_CONNECTED = -3,  ///< TCP connection to the server dropped.
    NOK_CLIENT_ERROR = -4,   ///< Server returned a 4xx client error.
    NOK_SERVER_ERROR = -5,   ///< Server returned a 5xx server error.
    NOK_INTERNAL_ERROR = -6  ///< Library reported a negative status code.
};

#define HTTPPort       80U        ///< Default HTTP port.
#define HTTPSPort      443U       ///< Default HTTPS port.
#define HTTPTimeout    20000UL    ///< Response timeout in milliseconds.
#define URL_BUFFER_SIZE 256U      ///< Maximum URL/path buffer size in bytes.

/**
 * @brief Allocates and configures an HTTP client instance.
 *
 * Creates a new @c HttpClient on the heap, bound to @p wifiClientInstance and
 * @p URL on port 80.  If the underlying WiFi socket is already open it is
 * closed first (unless @p overrideWifiInstance is @c false, in which case
 * NULL is returned to avoid interrupting an active connection).
 *
 * @param[in,out] wifiClientInstance  Underlying WiFiClient socket.
 * @param[in]     URL                 Hostname of the target server (no scheme).
 * @param[in]     overrideWifiInstance  If @c true, forcibly close an existing
 *                                    connection before creating the instance.
 * @return Pointer to the new @c HttpClient, or @c NULL on failure.
 * @note Caller is responsible for freeing via @c closeHTTPInstance().
 */
HttpClient *initializeHTTPInstance(WiFiClient &wifiClientInstance, const char *URL, bool overrideWifiInstance = false);

/**
 * @brief Allocates and configures an HTTPS client instance (port 443).
 *
 * Behaves identically to @c initializeHTTPInstance() but uses port 443.
 *
 * @param[in,out] wifiClientInstance  Underlying WiFiClient socket.
 * @param[in]     URL                 Hostname of the target server.
 * @param[in]     overrideWifiInstance  Force-close an existing connection.
 * @return Pointer to the new @c HttpClient, or @c NULL on failure.
 * @note Caller is responsible for freeing via @c closeHTTPInstance().
 */
HttpClient *initializeHTTPSInstance(WiFiClient &wifiClientInstance, const char *URL, bool overrideWifiInstance = false);

/**
 * @brief Performs an HTTP GET request and returns the response body.
 *
 * @param[in]  client   Active @c HttpClient (must not be NULL).
 * @param[in]  request  URL path and query string (e.g. @c "/api/data?id=1").
 * @param[out] payload  Response body on success; unchanged on error.
 * @return @c HTTPReturnStatus::OK on a 2xx response, or an appropriate error
 *         code for connection failures and non-2xx HTTP status codes.
 */
HTTPReturnStatus HTTPGet(HttpClient *client, const String request, String &payload);

/**
 * @brief Performs an HTTP POST request and returns the response body.
 *
 * @param[in]  client      Active @c HttpClient (must not be NULL).
 * @param[in]  request     URL path (e.g. @c "/api/submit").
 * @param[in]  contentType MIME type of the body (e.g. @c "application/json").
 * @param[in]  body        Request body string.
 * @param[out] payload     Response body on success; unchanged on error.
 * @return @c HTTPReturnStatus::OK on a 2xx response, or an appropriate error
 *         code for connection failures and non-2xx HTTP status codes.
 */
HTTPReturnStatus HTTPPost(HttpClient *client, const String request, const String contentType, const String body, String &payload);

/**
 * @brief Stops the connection and frees the @c HttpClient instance.
 *
 * @param[in] client Pointer to the instance to destroy.
 * @note Do not use @p client after this call — it is deleted.
 */
void closeHTTPInstance(HttpClient *client);

#endif
