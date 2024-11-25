#include "../include/GPSFunctions.h"
#include "../include/HTTPClientFunctions.h"

#define GPS_INIT_MAX_RETRY 3

bool initializeGPS(SFE_UBLOX_GNSS &myGNSS)
{
    bool initializationSuccess = false;
    pinMode(GPS_PIN, OUTPUT);
    digitalWrite(GPS_PIN, HIGH);

    for (int i = 0; i < GPS_INIT_MAX_RETRY; i++)
    {
        Serial1.begin(GPSBaudrate_Custom);
        if (myGNSS.begin(Serial1))
        {
            myGNSS.setUART1Output(COM_TYPE_UBX);
            if (myGNSS.getDynamicModel() != DYN_MODEL_AUTOMOTIVE)
                myGNSS.setDynamicModel(DYN_MODEL_AUTOMOTIVE);

            initializationSuccess = true;
            delay(1000);
            break;
        }

        Serial1.begin(GPSBaudrate_Default);
        if (myGNSS.begin(Serial1))
        {
            myGNSS.setSerialRate(GPSBaudrate_Custom);
            delay(1000);
        }
        else
        {
            // myGNSS.factoryReset();
            delay(1000);
        }
    }

    return initializationSuccess;
}

void getLatLongAlt(SFE_UBLOX_GNSS &myGNSS, float &latitude, float &longitude, float &altitude)
{
    latitude = (float)(myGNSS.getLatitude()) * 0.0000001;
    longitude = (float)(myGNSS.getLongitude()) * 0.0000001;
    altitude = (float)(myGNSS.getAltitudeMSL()) * 0.001;
}

void getSpeedHeading(SFE_UBLOX_GNSS &myGNSS, float &speed, float &heading)
{
    speed = (float)(myGNSS.getGroundSpeed()) * 0.0036; // km/h
    heading = (float)(myGNSS.getHeading()) * 0.00001;  // deg
}

bool setAcquisitionFrequency(SFE_UBLOX_GNSS &myGNSS, uint8_t rateHz)
{
    rateHz = rateHz >= 1 ? (rateHz < 10 ? rateHz : 10) : 1;
    return myGNSS.setNavigationFrequency(rateHz);
}

GPSSignalStrength evaluateSignal(SFE_UBLOX_GNSS &myGNSS)
{
    switch (myGNSS.getSIV())
    {
    case 0:
    case 1:
    case 2:
        // cannot get a position with only <2 satellites anyway
        return NOSIGNAL;
    case 3:
        // is this possible?
        return BAD;
    case 4:
    case 5:
        return AVERAGE;
    case 6:
    case 7:
    case 8:
        return GOOD;
    case 9:
    default:
        return EXCELLENT;
    }
}

void requestOnlineAssistNow(SFE_UBLOX_GNSS &myGNSS,HTTPClient &ubloxTS)
{
    /*!requests AssistNow(TM) online mode*/

    
#ifdef ROBUST_ASSISTNOW
    myGNSS.setAckAiding(1);
    myGNSS.pushAssistNowData(payload,payloadSize,SFE_UBLOX_MGA_ASSIST_ACK_ENQUIRE,10);
#else
    //myGNSS.pushAssistNowData(payload,payloadSize);
#endif
}

void requestOfflineAssistNow(SFE_UBLOX_GNSS &myGNSS)
{
    /*!requests AssistNow(TM) offline mode*/
}