#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H 1

//#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include "Adafruit_LEDBackpack.h"
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

#define STATUS_INDICATOR        LED_BUILTIN
#define OLED_SCREEN_HEIGT       32U
#define OLED_SCREEN_WIDTH       128U
#define OLED_RESET              -1
#define SCREEN_ADDRESS          0x3C

#define SEGLED_ADDRESS          0x70

#define GPS_PIN                 7
#define DEFAULT_GPS_BAUDRATE    9600U
#define CUSTOM_GPS_BAUDRATE     38400U
#define GPS_INIT_MAX_RETRY      3

#define SERVO_XAXIS_PIN         20
#define SERVO_YAXIS_PIN         19
#define SERVO_XAXIS_PWMIN       1
#define SERVO_XAXIS_PWMAX       2
#define SERVO_YAXIS_PWMIN       1
#define SERVO_YAXIS_PWMAX       2
#define MAX_XAXIS_ANGLE         180U
#define MAX_YAXIS_ANGLE         180U

#define PACKET_BYTE_LENGTH      20U
#define SERIAL_BAUDRATE         115200U
const char16_t PACKET_START_MARKER = 0x6752;
const char PACKET_END_MARKER = 0xED;

enum GPSSignalStrength{
    EXCELLENT,
    GOOD,
    AVERAGE,
    BAD,
    NOSIGNAL
};

inline int32_t div10Approx(int32_t number){
    /*!Will not check for overflow
    not accurate for large number
    */
    return (6554 * number) >> 16;
}

inline int32_t div100Approx(int32_t number){
    /*!Will not check for overflow
    not accurate for large number
    */
    return (10486 * number) >> 20;
}

inline int64_t div1000Approx(int64_t number){
    /*!Will not check for overflow
    not accurate for large number
    */
   return (536871 * number) >> 29;
}

bool initializeServo(Servo &servo_XAxis,Servo &servo_YAxis);
int readPosition(Servo &servo_XYAxis);
void writePosition(Servo &servo_XAxis,byte x,Servo &servo_YAxis,byte y);
void closeServo(Servo &servo_XAxis,Servo &servo_YAxis);

bool initializeSegmentLED(Adafruit_AlphaNum4 &alpha4);
void adjustLEDBrightness(Adafruit_AlphaNum4 &alpha4, uint8_t ambientLuminosity);
void writeFloatLED_Mirror(Adafruit_AlphaNum4 &alpha4,float number);
void writeStringLED_Mirror(Adafruit_AlphaNum4 &alpha4,const char *stringInput);

bool initializeGPS(SFE_UBLOX_GNSS &myGNSS);
void getLatLongAlt(SFE_UBLOX_GNSS &myGNSS,float &latitude,float &longitude,float &altitude);
void getSpeedHeading(SFE_UBLOX_GNSS &myGNSS,float &speed,float &heading);

#endif
