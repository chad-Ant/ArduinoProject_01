#include "include/GPSFunctions.h"
//#include "include/ServoFunctions.h"
#include "include/SegmentLEDFunctions.h"
//#include "include/WiFiFunctions.h"
//#include "include/HTTPClientFunctions.h"

//Adafruit_SSD1306 display(OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGT, &Wire, OLED_RESET);
//Servo servo_XAxis;
//Servo servo_YAxis;
SFE_UBLOX_GNSS myGNSS;
Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();
//HttpClient *ubloxTS = nullptr;

PinStatus LED_on = HIGH;

static unsigned long lastTime = 0;

void setup()
{
  //Debug setup
  //Serial.begin(SERIAL_BAUDRATE);
  //while (!Serial); //Wait for user to open terminal

  pinMode(STATUS_INDICATOR, OUTPUT);
  digitalWrite(STATUS_INDICATOR,HIGH);
  /**/

  //Servo setup
  //bool servoStartSuccess = initializeServo(servo_XAxis,servo_YAxis);
  
  //GPS Shield setup
  bool GPSStartSuccess = initializeGPS(myGNSS);

  //14-Segment setup
  bool SegLEDStartSuccess = initializeSegmentLED(alpha4);

  //Wifi setup
  //bool initializeWifi();

  //AssistNow GPS setup
  //ubloxTS = initializeHTTPInstance(AssistNowServer1);
}

void loop()
{
  static float speed,heading;
  //if (millis() - lastTime >= 125) 
  //{
    getSpeedHeading(myGNSS,speed,heading);
    writeFloatLED_Mirror(alpha4,speed);
    LED_on = LED_on == HIGH ? LOW : HIGH;
    digitalWrite(STATUS_INDICATOR,LED_on);
    //Serial.println(speed);
    //Serial.println(myGNSS.getSIV());
    //Serial.println("-------------");
    //lastTime = millis();
  //}
}