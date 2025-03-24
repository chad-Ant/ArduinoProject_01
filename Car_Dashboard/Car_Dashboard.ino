#include "config/DataDictionary.h"
#include "include/WiFiFunctions.h"
#include "include/HTTPClientFunctions.h"
#include "include/TimerFunctions.h" //works
#include "include/GPSFunctions.h"
#include "include/QuickMathFunctions.h"
#include "include/MathFunctions.h"

WiFiClient gpsServerConnection;
HttpClient *gpsClient = initializeHTTPInstance(gpsServerConnection, AssistNowServer1);


bool isgpsClientNull = !gpsClient;

SFE_UBLOX_GNSS myGNSS;

long lastTime = 0;
float lat, lon, alt;
float altFiltered;
PinStatus LED_on = HIGH;

SimpleMovingAverage altitudeFilter(SIZE_8);

void setup()
{
  pinMode(STATUS_INDICATOR, OUTPUT);
  digitalWrite(STATUS_INDICATOR,HIGH);

  //Debug setup
  Serial.begin(9600);
  while (!Serial); //Wait for user to open terminal

  if(isgpsClientNull) Serial.println("gpsClient is null!!!");

  //Wifi setup
  if (initializeWifi())
    Serial.println("Wifi connected.");
  else 
    Serial.println("Wifi not connected.");
  
  //GPS Shield setup
  if (initializeGPS(myGNSS)) Serial.println("GPS module started.");
  else Serial.println("GPS module failed");
  
  int GPSAssistResponse = requestOnlineAssistNow(myGNSS,gpsClient);
  if (GPSAssistResponse == 200) Serial.println("OnlineAssistNow success.");
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
  Serial.println(myGNSS.getSIV());
  Serial.println("-------------");
}