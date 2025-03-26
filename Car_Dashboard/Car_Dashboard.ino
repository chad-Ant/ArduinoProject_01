#include "config/DataDictionary.h"
#include "include/WiFiFunctions.h"
#include "include/HTTPClientFunctions.h"
#include "include/TimerFunctions.h" //works
#include "include/GPSFunctions.h"
#include "include/MathFunctions.h"

WiFiClient gpsServerConnection;
HttpClient *gpsClient = initializeHTTPInstance(gpsServerConnection, AssistNowServer1);
RTCZero rtc;

SFE_UBLOX_GNSS myGNSS;

unsigned long lastGPSUpdate, lastLEDBlink = 0;
float lat, lon, alt;
float altFiltered;
int LED_on = 1;

SimpleMovingAverage altitudeFilter(SIZE_8);

void setup()
{
  pinMode(STATUS_INDICATOR, OUTPUT);
  digitalWrite(STATUS_INDICATOR,LED_on);

  //Debug setup
  Serial.begin(9600);

  //Wifi setup
  if (initializeWifi() == WiFiReturnStatus::WIFI_SUCCESS) Serial.println("Wifi connected.");
  else Serial.println("Wifi not connected.");
  
  //RTC setup
  setRTCDateTime(rtc);

  //GPS Shield setup
  if (initializeGPS(myGNSS) == GPSReturnStatus::GPS_SUCCESS) Serial.println("GPS module started.");
  else Serial.println("GPS module failed");
  
  if (requestOnlineAssistNow(myGNSS,gpsClient) == GPSReturnStatus::ASSISTNOW_SUCCESS) Serial.println("OnlineAssistNow success.");
  else Serial.println("OnlineAssistNow failed.");
  
}

void loop()
{ 
  GPSReturnStatus status = getLatLongAlt(myGNSS,lat,lon,alt);
  if (isTimeout(500,lastGPSUpdate)){
    if (status == DATA_STALE) Serial.println("data is stale");
    Serial.println(myGNSS.getSIV());
    Serial.println("-------------");
    lastGPSUpdate = millis();
  }
  
  if (isTimeout(1000,lastLEDBlink)){
    LED_on ^= 1;
    digitalWrite(STATUS_INDICATOR,LED_on);
    lastLEDBlink = millis();
  }
}