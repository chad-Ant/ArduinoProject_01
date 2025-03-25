#include "config/DataDictionary.h"
#include "include/WiFiFunctions.h"
#include "include/HTTPClientFunctions.h"
#include "include/TimerFunctions.h" //works
#include "include/GPSFunctions.h"
#include "include/MathFunctions.h"

WiFiClient gpsServerConnection;
HttpClient *gpsClient = initializeHTTPInstance(gpsServerConnection, AssistNowServer1);

bool isgpsClientNull = !gpsClient;

SFE_UBLOX_GNSS myGNSS;

unsigned long lastGPSUpdate, lastLEDBlink = millis();
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
  while (!Serial); //Wait for user to open terminal

  if(isgpsClientNull) Serial.println("gpsClient is null!!!");

  //Wifi setup
  if (initializeWifi()) Serial.println("Wifi connected.");
  else Serial.println("Wifi not connected.");
  
  //GPS Shield setup
  if (initializeGPS(myGNSS)) Serial.println("GPS module started.");
  else Serial.println("GPS module failed");
  
  if (requestOnlineAssistNow(myGNSS,gpsClient)) Serial.println("OnlineAssistNow success.");
  else Serial.println("OnlineAssistNow failed.");
  
}

void loop()
{ 
  getLatLongAlt(myGNSS,lat,lon,alt);
  if (altitudeFilter.calculate(alt,altFiltered)) {
  Serial.print("filtered value: ");
  Serial.println(altFiltered);
  Serial.print("raw value: ");
  Serial.println(alt);
  } else {
    Serial.println(alt);
    Serial.println("Filter not warmed up yet.");
  }
  Serial.print("lat: ");
  Serial.print(lat);
  Serial.print(", lon: ");
  Serial.print(lon);
  uint8_t SIV = myGNSS.getSIV();
  Serial.println(SIV);
  Serial.println("-------------");
  
  LED_on ^= 1;
  digitalWrite(STATUS_INDICATOR,LED_on);
  lastLEDBlink = millis();
}