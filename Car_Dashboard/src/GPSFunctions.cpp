#include "../include/GPSFunctions.h"
#include "../include/HTTPClientFunctions.h"
#include "../include/TimerFunctions.h"
#define GPS_INIT_MAX_RETRY 3

bool initializeGPS(SFE_UBLOX_GNSS &myGNSS){
    bool initializationSuccess = false;
    unsigned long startTime = millis();
    pinMode(GPS_PIN, OUTPUT);
    digitalWrite(GPS_PIN, HIGH);

    for (int i = 0; i < GPS_INIT_MAX_RETRY; i++){
        Serial1.begin(GPSBaudrate_Custom);
        if (myGNSS.begin(Serial1)){
#ifndef ENABLE_NMEA
            myGNSS.setUART1Output(COM_TYPE_UBX);
#endif
            if (myGNSS.getDynamicModel() != DYN_MODEL_AUTOMOTIVE)
                myGNSS.setDynamicModel(DYN_MODEL_AUTOMOTIVE);

            initializationSuccess = true;
            while(!isTimeout(1000,startTime));
            startTime = millis();
            break;
        }

        Serial1.begin(GPSBaudrate_Default);
        if (myGNSS.begin(Serial1)){
            myGNSS.setSerialRate(GPSBaudrate_Custom);
        }
        else{
            // myGNSS.factoryReset();
        }
        while(!isTimeout(1000,startTime));
    }

    return initializationSuccess;
}

bool intializeGPS_I2C(SFE_UBLOX_GNSS &myGNSS){

}

void getLatLongAlt(SFE_UBLOX_GNSS &myGNSS, float &latitude, float &longitude, float &altitude){
    latitude = (float)(myGNSS.getLatitude()) * 0.0000001;
    longitude = (float)(myGNSS.getLongitude()) * 0.0000001;
    altitude = (float)(myGNSS.getAltitudeMSL()) * 0.001;
}

void getSpeedHeading(SFE_UBLOX_GNSS &myGNSS, float &speed, float &heading){
    speed = (float)(myGNSS.getGroundSpeed()) * 0.0036; // km/h
    heading = (float)(myGNSS.getHeading()) * 0.00001;  // deg
}

bool setAcquisitionFrequency(SFE_UBLOX_GNSS &myGNSS, uint8_t rateHz){
    rateHz = rateHz >= 1 ? (rateHz < 10 ? rateHz : 10) : 1;
    return myGNSS.setNavigationFrequency(rateHz);
}

GPSSignalStrength evaluateSignal(SFE_UBLOX_GNSS &myGNSS){
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
    default:
        return EXCELLENT;
    }
}

void requestOnlineAssistNow(SFE_UBLOX_GNSS &myGNSS,HttpClient *ubloxTS,const AssistNowServer server){
    /*!requests AssistNow(TM) online mode*/
    char requestBuffer[160] = "";
    if (server == ONLINE1){
        sprintf(requestBuffer,GETRequest_Online,AssistNowServer1,AssistNowToken);
    } 
    else{
        sprintf(requestBuffer,GETRequest_Online,AssistNowServer2,AssistNowToken);
    }

    String payload = HTTPGet(ubloxTS,requestBuffer,"INVALID");
    if (payload != "INVALID" && payload.length() > 0){   
#ifdef ROBUST_ASSISTNOW
        myGNSS.setAckAiding(1);
        myGNSS.pushAssistNowData(payload,payload.length(),SFE_UBLOX_MGA_ASSIST_ACK_ENQUIRE,100);
#else
        myGNSS.pushAssistNowData(payload,payload.length());
#endif
    }
}

void requestOfflineAssistNow(SFE_UBLOX_GNSS &myGNSS, HttpClient *ubloxTS)
{
    /*!requests AssistNow(TM) offline mode*/
    char requestBuffer[160] = "";
    if (server == ONLINE1){
        sprintf(requestBuffer,GETRequest_Offline,AssistNowServer1,AssistNowToken);
    } 
    else{
        sprintf(requestBuffer,GETRequest_Offline,AssistNowServer2,AssistNowToken);
    }

    String payload = HTTPGet(ubloxTS,requestBuffer,"INVALID");
    if (payload != "INVALID" && payload.length() > 0){   
#ifdef ROBUST_ASSISTNOW
        myGNSS.setAckAiding(1);
        myGNSS.pushAssistNowData(payload,payload.length(),SFE_UBLOX_MGA_ASSIST_ACK_ENQUIRE,100);
#else
        myGNSS.pushAssistNowData(payload,payload.length());
#endif
    }
}