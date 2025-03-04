#include "include/GPSFunctions.h"
#include "include/ServoFunctions.h"
#include "include/SegmentLEDFunctions.h"
#include "include/WiFiFunctions.h"

//Adafruit_SSD1306 display(OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGT, &Wire, OLED_RESET);
Servo servo_XAxis;
Servo servo_YAxis;
SFE_UBLOX_GNSS myGNSS;

long lastTime = 0;
float lat, lon, alt;

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();
PinStatus LED_on = HIGH;

int i = 0;

void setup()
{
  //Debug setup
  Serial.begin(SERIAL_BAUDRATE);
  while (!Serial); //Wait for user to open terminal

  pinMode(STATUS_INDICATOR, OUTPUT);
  digitalWrite(STATUS_INDICATOR,HIGH);
  /**/

  //Servo setup
  bool servoStartSuccess = initializeServo(servo_XAxis,servo_YAxis);
  
  //GPS Shield setup
  bool GPSStartSuccess = initializeGPS(myGNSS);

  //14-Segment setup
  bool SegLEDStartSuccess = initializeSegmentLED(alpha4);

  //Wifi setup
  bool WifiStartSuccess = initializeWifi();
}

void loop()
{ 
  getLatLongAlt(myGNSS,lat,lon,alt);
  writeFloatLED_Mirror(alpha4,alt);
  Serial.println(alt);
  Serial.println(myGNSS.getSIV());
  Serial.println("-------------");
}