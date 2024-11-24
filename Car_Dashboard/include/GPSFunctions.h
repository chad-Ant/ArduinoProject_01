#ifndef GPS_FUNCTIONS
#define GPS_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

enum GPSSignalStrength{
    EXCELLENT,
    GOOD,
    AVERAGE,
    BAD,
    NOSIGNAL
};

bool initializeGPS(SFE_UBLOX_GNSS &myGNSS);
void getLatLongAlt(SFE_UBLOX_GNSS &myGNSS,float &latitude,float &longitude,float &altitude);
void getSpeedHeading(SFE_UBLOX_GNSS &myGNSS,float &speed,float &heading);
bool setAcquisitionFrequency(SFE_UBLOX_GNSS &myGNSS,uint8_t rateHz);
GPSSignalStrength evaluateSignal(SFE_UBLOX_GNSS &myGNSS);
void requestAssistNow(SFE_UBLOX_GNSS &myGNSS);

#endif