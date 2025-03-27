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

unsigned long lastGPSUpdate = 0, lastLEDBlink = 0, lastShowTime = 0;
float lat, lon, alt;
float altFiltered;
int LED_on = 1;
uint8_t tHr = 0, tMin = 0;
uint16_t tSec = 0;

IPAddress localIP;
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
  initializeRTC(rtc);
  if (setRTCDateTime(rtc)) Serial.println("RTC set!");
  else Serial.println("RTC not set up.");
  getRTCDate(rtc, tHr, tMin, tSec);
  Serial.print(tHr);
  Serial.print(":");
  Serial.print(tMin);
  Serial.print(":");
  Serial.println(tSec);


  //GPS Shield setup
  if (initializeGPS(myGNSS) == GPSReturnStatus::GPS_SUCCESS) Serial.println("GPS module started.");
  else Serial.println("GPS module failed");
  
  if (requestOnlineAssistNow(myGNSS,gpsClient) == GPSReturnStatus::GPS_ASSISTNOW_SUCCESS) Serial.println("OnlineAssistNow success.");
  else Serial.println("OnlineAssistNow failed.");
  
}

void loop()
{ 
  if (getLatLongAlt(myGNSS,lat,lon,alt) == GPS_DATA_FRESH){
    Serial.print("CURRENT SIV: ");
    Serial.println(myGNSS.getSIV());
  }
  
  if (isTimeout(2000,lastLEDBlink)){
    LED_on ^= 1;
    digitalWrite(STATUS_INDICATOR,LED_on);
    lastLEDBlink = millis();
  }

  if (isTimeout(100,lastShowTime)){
    Serial.print(".");
    lastShowTime = millis();
  }
}