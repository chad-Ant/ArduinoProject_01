#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H 1

//#ifdef __cplusplus
//extern "C" {
//#endif

#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
//#include <Adafruit_SSD1306.h>
#include <Servo.h>

#define STATUS_INDICATOR        LED_BUILTIN
#define OLED_SCREEN_HEIGT       32U
#define OLED_SCREEN_WIDTH       128U
#define OLED_RESET              -1
#define SCREEN_ADDRESS          0x3C

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

typedef enum ServoAxis{
    X_AXIS = 0,
    Y_AXIS = 1
}ServoAxis;

#define PACKET_BYTE_LENGTH      20U
#define SERIAL_BAUDRATE         115200U
const char16_t PACKET_START_MARKER = 0x6752;
const char PACKET_END_MARKER = 0xED;


bool initializeServo(Servo &servo_XAxis,Servo &servo_YAxis);
int readPosition(Servo &servo_XYAxis);
void writePosition(Servo &servo_XAxis,byte x,Servo &servo_YAxis,byte y);
void closeServo(Servo &servo_XAxis,Servo &servo_YAxis);

bool initializeGPS(SFE_UBLOX_GNSS &myGNSS);

//#ifdef __cplusplus
//}
//#endif

#endif
