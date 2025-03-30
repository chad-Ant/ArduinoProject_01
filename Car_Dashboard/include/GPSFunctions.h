#ifndef GPS_FUNCTIONS
#define GPS_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

//GPS module uses Serial1 interface for MKR1000 WiFi, dedicated I2C interface for MKR ZERO

static_assert(GPS_ASSISTNOW_TOKEN[0] != '\0', "GPS_ASSISTNOW_TOKEN missing, please add token from u-blox website before compilation.");

enum class GPSReturnStatus{
    OK = 0,
    NOK_INIT_FAILED = -1,
    DATA_STALE = 1,
    NOK_SET_RATE_FAILED = -2,
    NOK_AN_REQUEST_FAILED = -3,
    NOK_AN_PUSH_FAILED = -4
};

enum GPSSignalStrength{
    EXCELLENT,
    GOOD,
    AVERAGE,
    BAD,
    NOSIGNAL
};

enum AssistNowServer{
    ONLINE1,
    ONLINE2
};

GPSReturnStatus initializeGPS(SFE_UBLOX_GNSS &myGNSS);
GPSReturnStatus intializeGPS_I2C(SFE_UBLOX_GNSS &myGNSS);
GPSReturnStatus getLatLongAlt(SFE_UBLOX_GNSS &myGNSS,float &latitude,float &longitude,float &altitude); //must be called as fast as possible to avoid buffer overflow
GPSReturnStatus getSpeedHeading(SFE_UBLOX_GNSS &myGNSS,float &speed,float &heading); //must be called as fast as possible to avoid buffer overflow
GPSReturnStatus setAcquisitionFrequency(SFE_UBLOX_GNSS &myGNSS,uint8_t rateHz);
GPSSignalStrength evaluateSignal(SFE_UBLOX_GNSS &myGNSS);
GPSReturnStatus requestOnlineAssistNow(SFE_UBLOX_GNSS &myGNSS,HttpClient *ubloxTS);
#endif