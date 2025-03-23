#ifndef GPS_FUNCTIONS
#define GPS_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

//static_assert(AssistNowToken[0] != '\0', "AssistNowToken missing, please add token from u-blox website before compilation.");

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

bool initializeGPS(SFE_UBLOX_GNSS &myGNSS);
bool intializeGPS_I2C(SFE_UBLOX_GNSS &myGNSS);
void getLatLongAlt(SFE_UBLOX_GNSS &myGNSS,float &latitude,float &longitude,float &altitude);
void getSpeedHeading(SFE_UBLOX_GNSS &myGNSS,float &speed,float &heading);
bool setAcquisitionFrequency(SFE_UBLOX_GNSS &myGNSS,uint8_t rateHz);
GPSSignalStrength evaluateSignal(SFE_UBLOX_GNSS &myGNSS);
void requestOnlineAssistNow(SFE_UBLOX_GNSS &myGNSS,HttpClient *ubloxTS,const AssistNowServer server);
void requestOfflineAssistNow(SFE_UBLOX_GNSS &myGNSS,HttpClient *ubloxTS,const AssistNowServer server);
#endif