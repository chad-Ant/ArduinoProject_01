#include "include/HelperFunctions.h"
#include <CAN.h>
#include <Wire.h>
#include <Servo.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

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
  //Serial1 setup
  Serial.begin(SERIAL_BAUDRATE);
  while (!Serial); //Wait for user to open terminal

  pinMode(STATUS_INDICATOR, OUTPUT);
  digitalWrite(STATUS_INDICATOR,HIGH);
  /*
  //OLED setup
  if(initializeOLED(display) && initializeServo(servo_XAxis,servo_YAxis) && initializeGPS(myGNSS)){
    Serial.println("Starting stuff!");
  }
*/
  //Servo setup
  bool servoStartSuccess = initializeServo(servo_XAxis,servo_YAxis);
  
  //GPS Shield setup
  bool GPSStartSuccess = initializeGPS(myGNSS);

  bool SegLEDStartSuccess = initializeSegmentLED(alpha4);

  /*CAN interrupt setup
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
  */
  // register the receive callback
  //CAN.onReceive(onReceive);
}

void loop()
{ 
  getLatLongAlt(myGNSS,lat,lon,alt);
  writeFloatLED_Mirror(alpha4,alt);
  //writeStringLED_Mirror(alpha4,"err9");
  Serial.println(alt);
  Serial.println(myGNSS.getSIV());
  Serial.println("-------------");
}