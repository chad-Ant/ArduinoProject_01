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
bool LEDStatus = false;
char16_t displayString[128];

void setup()
{
  //Serial1 setup
  Serial.begin(SERIAL_BAUDRATE);
  while (!Serial); //Wait for user to open terminal

  pinMode(STATUS_INDICATOR, OUTPUT);

  /*
  //OLED setup
  if(initializeOLED(display) && initializeServo(servo_XAxis,servo_YAxis) && initializeGPS(myGNSS)){
    Serial.println("Starting stuff!");
  }
*/
  //Servo setup
  bool servoStartSuccess;
  servoStartSuccess = initializeServo(servo_XAxis,servo_YAxis);
  
  //GPS Shield setup
  bool GPSStartSuccess;
  GPSStartSuccess = initializeGPS(myGNSS);

  //CAN interrupt setup
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
  // register the receive callback
  //CAN.onReceive(onReceive);
}

void loop()
{ 
}