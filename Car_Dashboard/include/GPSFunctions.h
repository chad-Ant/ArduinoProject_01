#ifndef GPS_FUNCTIONS
#define GPS_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

static_assert(AssistNowToken[0] != '\0', "AssistNowToken missing, please add token from u-blox website before compilation.");

enum GPSReturnStatus{
    GPS_SUCCESS,
    GPS_FAILED,
    DATA_STALE,
    DATA_FRESH,
    SET_GPS_RATE_SUCCESS,
    SET_GPS_RATE_FAILED,
    ASSISTNOW_SUCCESS,
    ASSISTNOW_REQUEST_FAILED,
    ASSISTNOW_PUSH_FAILED
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