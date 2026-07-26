#ifndef WIFI_FUNCTIONS
#define WIFI_FUNCTIONS 1

#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>   // WiFi, WiFiClient, IPAddress — MKR 1000 WiFi radio
#include <RTCZero.h>   // setRTCDateTime() takes an RTCZero&
#include "../config/DataDictionary.h"

/// WiFi runs on SPI1 (WiFi101 shield).
#define WIFI_MAX_RETRY 3U
/// Milliseconds to wait for each connection attempt before trying the next SSID.
#define WIFI_WAIT_MSEC 5000UL

/** Return codes used by WiFi functions. */
enum class WiFiReturnStatus{
    OK = 0,            ///< Operation succeeded.
    NOK_NO_SHIELD = -1,      ///< WiFi module not detected.
    NOK_NO_CONNECTION = -2,  ///< Could not associate with any known SSID.
    NOK_EMPTY_IP = -3,       ///< Resulting IP address is empty/unset.
    NOK_EMPTY_DNS = -4,      ///< DNS address is empty/unset.
    NOK_IP_MISMATCH = -5,    ///< Configured IP does not match what the module reported.
    NOK_ZERO_TIME = -6,      ///< NTP returned Unix time of zero.
    NOK_CLOSE_FAILED = -7,   ///< WiFi did not disconnect after request.
    NOK_RTC_FAILED = -8,     ///< RTCZero is not configured (required for NTP→RTC sync).
};

/**
 * @brief Connects to WiFi using the credentials defined in DataDictionary.h.
 *
 * Tries up to three SSIDs in order (primary, backup-1, backup-2), waiting
 * @c WIFI_WAIT_MSEC between each attempt.
 *
 * @return @c WiFiReturnStatus::OK on success, @c NOK_NO_SHIELD if the module
 *         is absent, or @c NOK_NO_CONNECTION if all SSIDs fail.
 */
WiFiReturnStatus initializeWifi();

/**
 * @brief Assigns a static IP and DNS address to the WiFi interface.
 *
 * @param[in] ip  Desired static IP address.
 * @param[in] dns Desired DNS server address.
 * @return @c WiFiReturnStatus::OK if the IP was applied and verified,
 *         or an appropriate error code on failure.
 * @note Using a static IP will break @c getUnixTime() because the WiFi101
 *       NTP helper requires DHCP.
 */
WiFiReturnStatus configStaticIP(const IPAddress ip, const IPAddress dns);

/**
 * @brief Checks whether the WiFi module is currently associated with an AP.
 * @return @c true if connected, @c false otherwise.
 */
bool isWifiConnected();

/**
 * @brief Retrieves the current Unix timestamp from the WiFi101 NTP service.
 *
 * @param[out] unixTime Seconds since the Unix epoch (UTC).
 * @return @c WiFiReturnStatus::OK on success, @c NOK_NO_CONNECTION if WiFi
 *         is down, or @c NOK_ZERO_TIME if the module returned 0.
 * @note This call blocks briefly while the module contacts the NTP server.
 *       Static IP configuration will cause it to fail — use DHCP instead.
 */
WiFiReturnStatus getUnixTime(unsigned long &unixTime);

/**
 * @brief Returns the local IP address assigned to the WiFi interface.
 *
 * @param[out] localIP Current local IPv4 address.
 * @return @c WiFiReturnStatus::OK, @c NOK_NO_CONNECTION, or @c NOK_EMPTY_IP.
 */
WiFiReturnStatus getLocalIP(IPAddress &localIP);

/**
 * @brief Fetches the current date (UTC + timezone offset) from the WiFi101 NTP service.
 *
 * @param[out] tDate    Day of month (1–31).
 * @param[out] tMonth   Month (1–12).
 * @param[out] tYear    Four-digit year (e.g. 2025).
 * @param[in]  timezone UTC offset in whole hours, clamped to [−12, +12].
 *                      Fractional-hour zones are not supported.
 * @return @c WiFiReturnStatus::OK, @c NOK_NO_CONNECTION, or @c NOK_ZERO_TIME.
 */
WiFiReturnStatus getNTPDate(uint8_t &tDate, uint8_t &tMonth, uint16_t &tYear, int8_t timezone = 0);

/**
 * @brief Fetches the current time (UTC + timezone offset) from the WiFi101 NTP service.
 *
 * @param[out] tHour    Hours (0–23).
 * @param[out] tMinute  Minutes (0–59).
 * @param[out] tSecond  Seconds (0–59).
 * @param[in]  timezone UTC offset in whole hours, clamped to [−12, +12].
 * @return @c WiFiReturnStatus::OK, @c NOK_NO_CONNECTION, or @c NOK_ZERO_TIME.
 */
WiFiReturnStatus getNTPTime(uint8_t &tHour, uint8_t &tMinute, uint8_t &tSecond, int8_t timezone = 0);

/**
 * @brief Fetches the current date and time in a single NTP call.
 *
 * Performs only one underlying @c getUnixTime() call, then decomposes the
 * result into both date and time fields.
 *
 * @param[out] tHour    Hours (0–23).
 * @param[out] tMinute  Minutes (0–59).
 * @param[out] tSecond  Seconds (0–59).
 * @param[out] tDate    Day of month (1–31).
 * @param[out] tMonth   Month (1–12).
 * @param[out] tYear    Four-digit year.
 * @param[in]  timezone UTC offset in whole hours, clamped to [−12, +12].
 * @return @c WiFiReturnStatus::OK, @c NOK_NO_CONNECTION, or @c NOK_ZERO_TIME.
 */
WiFiReturnStatus getNTPDateTime(uint8_t &tHour, uint8_t &tMinute, uint8_t &tSecond,
                                uint8_t &tDate, uint8_t &tMonth, uint16_t &tYear,
                                int8_t timezone = 0);

/**
 * @brief Sets the RTCZero clock from the current NTP time.
 *
 * Obtains the Unix timestamp via WiFi, applies the timezone offset, then
 * programs the RTCZero date and time registers.
 *
 * @param[in,out] rtc      RTCZero instance (must already be initialised).
 * @param[in]     timezone UTC offset in whole hours (default @c LOCAL_TIMEZONE).
 * @return @c WiFiReturnStatus::OK, @c NOK_NO_CONNECTION, @c NOK_ZERO_TIME, or
 *         @c NOK_RTC_FAILED.
 */
WiFiReturnStatus setRTCDateTime(RTCZero &rtc, int8_t timezone = LOCAL_TIMEZONE);

/**
 * @brief Disconnects from the current access point.
 * @return @c WiFiReturnStatus::OK on success, @c NOK_CLOSE_FAILED if the
 *         module is still reporting connected after the call.
 */
WiFiReturnStatus closeWifi();

/**
 * @brief Disconnects and powers down the WiFi module.
 * @note Use this to save power when WiFi is not needed for an extended period.
 */
void turnoffWifi();

#endif
