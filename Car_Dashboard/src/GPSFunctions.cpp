#include "../include/GlobalVariables.h"
#include "../include/GPSFunctions.h"
#include "../include/HTTPClientFunctions.h"
#include "../include/TimerFunctions.h"
#define GPS_INIT_MAX_RETRY 3

GPSReturnStatus initializeGPS(SFE_UBLOX_GNSS &myGNSS){
    pinMode(GPS_PIN, OUTPUT);
    digitalWrite(GPS_PIN, HIGH);

    //1st attempt with custom baud rate
    Serial1.begin(GPS_BAUDRATE_CUSTOM);
    if (myGNSS.begin(Serial1)){
#ifndef GPS_ENABLE_NMEA
        myGNSS.setUART1Output(COM_TYPE_UBX);
#endif
        if (myGNSS.getDynamicModel() != DYN_MODEL_AUTOMOTIVE) myGNSS.setDynamicModel(DYN_MODEL_AUTOMOTIVE);
        myGNSS.setNavigationFrequency(GPS_REFRESH_RATE);
        myGNSS.setNavigationRate(1); //one nav solution per update
        myGNSS.setAutoPVTrate(1);
        return GPS_SUCCESS;
    }
    
    //set baud rate to custom baud rate for high resolution positioning
    Serial1.begin(GPS_BAUDRATE_DEFAULT);
    if (myGNSS.begin(Serial1)){
        myGNSS.setSerialRate(GPS_BAUDRATE_CUSTOM);
        myGNSS.saveConfiguration();
    }
    //2nd attempt with custom baud rate
    Serial1.begin(GPS_BAUDRATE_CUSTOM);
    if (myGNSS.begin(Serial1)){
#ifndef GPS_ENABLE_NMEA
        myGNSS.setUART1Output(COM_TYPE_UBX);
#endif
        if (myGNSS.getDynamicModel() != DYN_MODEL_AUTOMOTIVE) myGNSS.setDynamicModel(DYN_MODEL_AUTOMOTIVE);
        myGNSS.setNavigationFrequency(GPS_REFRESH_RATE);
        myGNSS.setNavigationRate(1); //one nav solution per update
        myGNSS.setAutoPVTrate(1);
        return GPS_SUCCESS;
    }

    //use the default options if failed to set up high res positioning
    Serial1.begin(GPS_BAUDRATE_DEFAULT);
    if (myGNSS.begin(Serial1)){
        #ifndef GPS_ENABLE_NMEA
        myGNSS.setUART1Output(COM_TYPE_UBX);
#endif
        if (myGNSS.getDynamicModel() != DYN_MODEL_AUTOMOTIVE) myGNSS.setDynamicModel(DYN_MODEL_AUTOMOTIVE);
        myGNSS.setNavigationFrequency(1); //1Hz, safest option here
        myGNSS.setNavigationRate(1); //one nav solution per update
        myGNSS.setAutoPVTrate(1); 
        return GPS_SUCCESS;
    }

    //give up lmao
    return GPS_FAILED;
}

GPSReturnStatus intializeGPS_I2C(SFE_UBLOX_GNSS &myGNSS){
    if (!i2cInitialized){
        Wire.begin();
        i2cInitialized = true;
    }

    if (!myGNSS.begin(Wire,GPS_DEFAULT_I2C_ADDRESS) || !myGNSS.begin(Wire,GPS_BACKUP_I2C_ADDRESS)) return GPS_FAILED;
    delay(500);
#ifndef GPS_ENABLE_NMEA
    myGNSS.setI2COutput(COM_TYPE_UBX);
#endif
    if (myGNSS.getDynamicModel() != DYN_MODEL_AUTOMOTIVE) myGNSS.setDynamicModel(DYN_MODEL_AUTOMOTIVE);
    myGNSS.setI2CpollingWait(25);
    myGNSS.setNavigationFrequency(GPS_REFRESH_RATE);
    myGNSS.setNavigationRate(1); //one nav solution per update
    myGNSS.setAutoPVTrate(1);
    return GPS_SUCCESS;
}

GPSReturnStatus getLatLongAlt(SFE_UBLOX_GNSS &myGNSS, float &latitude, float &longitude, float &altitude){
    if (!myGNSS.getPVT()) return GPS_DATA_STALE;
    latitude = (float)(myGNSS.getLatitude()) * 0.0000001;
    longitude = (float)(myGNSS.getLongitude()) * 0.0000001;
    altitude = (float)(myGNSS.getAltitudeMSL()) * 0.001;
    return GPS_DATA_FRESH;
}

GPSReturnStatus getSpeedHeading(SFE_UBLOX_GNSS &myGNSS, float &speed, float &heading){
    if (!myGNSS.getPVT()) return GPS_DATA_STALE;
    speed = (float)(myGNSS.getGroundSpeed()) * 0.0036; // km/h
    heading = (float)(myGNSS.getHeading()) * 0.00001;  // deg
    return GPS_DATA_FRESH;
}

GPSReturnStatus setAcquisitionFrequency(SFE_UBLOX_GNSS &myGNSS, uint8_t rateHz){
    rateHz = rateHz >= 1 ? (rateHz < 10 ? rateHz : 10) : 1;
    return myGNSS.setNavigationFrequency(rateHz) ? GPS_SET_RATE_SUCCESS : GPS_SET_RATE_FAILED;
}

GPSSignalStrength evaluateSignal(SFE_UBLOX_GNSS &myGNSS){
    switch (myGNSS.getSIV())
    {
    case 0:
    case 1:
    case 2:
        // cannot get a position with only <=2 satellites anyway
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

GPSReturnStatus requestOnlineAssistNow(SFE_UBLOX_GNSS &myGNSS,HttpClient *ubloxTS){
    /*!requests AssistNow(TM) online mode*/
    char requestBuffer[256] = "";
    sprintf(requestBuffer,GPS_GET_ASSISTNOW_ONLINE,GPS_ASSISTNOW_TOKEN,GPS_DEFAULT_POSITION);
    String payload = "";
    if (HTTPGet(ubloxTS,requestBuffer,payload) != HTTP_COMMAND_SUCCESS) return GPS_ASSISTNOW_REQUEST_FAILED; 
#ifdef ROBUST_ASSISTNOW
    myGNSS.setAckAiding(1);
    if (myGNSS.pushAssistNowData(payload,payload.length(),SFE_UBLOX_MGA_ASSIST_ACK_ENQUIRE,100) > 0) return GPS_ASSISTNOW_SUCCESS;
#else
    if (myGNSS.pushAssistNowData(payload,payload.length()) > 0) return GPS_ASSISTNOW_SUCCESS;
#endif
    return GPS_ASSISTNOW_PUSH_FAILED;
}

//MKR mControllers don't have enough memory to store AssistNow Offline data
//This function is not used in the current implementation
//If a different HTTP library can split the response into smaller chunks, 
//with some modifications, this function can be used

/*
int requestOfflineAssistNow(SFE_UBLOX_GNSS &myGNSS, HttpClient *ubloxTS){
    //!requests AssistNow(TM) offline mode
    char requestBuffer[256] = "";
    sprintf(requestBuffer,GETRequest_Offline,GPS_ASSISTNOW_TOKEN);
    String payload = "";
    int responseCode = HTTPGet(ubloxTS,requestBuffer,payload);
    if (responseCode != 200 || payload.length() == 0) return responseCode;

    uint8_t month = 0, day = 0, hour = 0, minute = 0, second = 0;
    uint16_t year = 0;
    size_t todayDataIndex = 0, tomorrowDataIndex = payload.length();
    if (!getNTPDateTime(hour,minute,second,day,month,year)) return -1;
    Serial.println(year);
    todayDataIndex = myGNSS.findMGAANOForDate(payload,payload.length(),year,month,day);
    tomorrowDataIndex = myGNSS.findMGAANOForDate(payload,payload.length(),year,month,day,1);
    myGNSS.setUTCTimeAssistance(year,month,day,hour,minute,second);
    myGNSS.pushAssistNowData(todayDataIndex,true,payload,tomorrowDataIndex - todayDataIndex);
    return responseCode;
}
*/