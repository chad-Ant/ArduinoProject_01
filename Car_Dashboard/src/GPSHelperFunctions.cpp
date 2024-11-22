#include "..\include\HelperFunctions.h"
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <Arduino.h>

bool initializeGPS(SFE_UBLOX_GNSS &myGNSS)
{
    bool initializationSuccess = false;
    pinMode(GPS_PIN, OUTPUT);
    digitalWrite(GPS_PIN, HIGH);

    for (int i = 0; i < GPS_INIT_MAX_RETRY; i++)
    {
        Serial1.begin(CUSTOM_GPS_BAUDRATE);
        if (myGNSS.begin(Serial1))
        {
            myGNSS.setUART1Output(COM_TYPE_UBX);
            initializationSuccess = true;
            break;
        }
        delay(1000);

        Serial1.begin(DEFAULT_GPS_BAUDRATE);
        if (myGNSS.begin(Serial1))
        {
            myGNSS.setSerialRate(CUSTOM_GPS_BAUDRATE);
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

void getLatLongAlt(SFE_UBLOX_GNSS &myGNSS,float &latitude,float &longitude,float &altitude){
    latitude = (float)(myGNSS.getLatitude()) * 0.0000001;
    longitude = (float)(myGNSS.getLongitude()) * 0.0000001;
    altitude = (float)(myGNSS.getAltitudeMSL()) * 0.001;

}

void getSpeedHeading(SFE_UBLOX_GNSS &myGNSS,float &speed,float &heading){
    speed = (float)(myGNSS.getGroundSpeed()) * 0.0036;  //km/h
    heading = (float)(myGNSS.getHeading()) * 0.00001;   //deg
}
/*
void sendData(int latitude,int longitude,int altitude,int speed, int heading){
}

void int32ToChar16String(uint32_t number,char16_t *output,size_t outputSize){
    if (outputSize == 0 || output == nullptr) return;
    size_t index = 0;

    if (number < 0) {
        if (index < outputSize - 1) {
            output[index++] = u'-';
        } else {
            // Not enough space to store the minus sign
            output[0] = u'\0';
            return;
        }
        number = -number;
    }

    char16_t tempBuffer[12]; // Buffer to hold digits in reverse
    size_t tempIndex = 0;

    // Handle zero explicitly
    if (number == 0) {
        tempBuffer[tempIndex++] = u'0';
    } else {
        while (number > 0 && tempIndex < sizeof(tempBuffer)/sizeof(tempBuffer[0])) {
            int digit = number % 10;
            tempBuffer[tempIndex++] = u'0' + digit;
            number /= 10;
        }
    }

    while (tempIndex > 0 && index < outputSize - 1) {
        output[index++] = tempBuffer[--tempIndex];
    }

    output[index] = u'\0'; // Null-terminate the string
}
*/