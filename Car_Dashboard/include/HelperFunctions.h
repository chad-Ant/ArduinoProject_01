#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H 1

#ifdef __cplusplus
extern "C" {
#endif

#define STATUS_INDICATOR        LED_BUILTIN
#define OLED_SCREEN_HEIGT       32U
#define OLED_SCREEN_WIDTH       128U
#define OLED_RESET              -1
#define SCREEN_ADDRESS          0x3C

#define GPS_PIN                 7
#define DEFAULT_GPS_BAUDRATE    9600U
#define CUSTOM_GPS_BAUDRATE     38400U

#define SERVO_XAXIS_PIN         20
#define SERVO_YAXIS_PIN         19
#define SERVO_XAXIS_PWMIN       1
#define SERVO_XAXIS_PWMAX       2
#define SERVO_YAXIS_PWMIN       1
#define SERVO_YAXIS_PWMAX       2
#define MAX_XAXIS_ANGLE         360U
#define MAX_YAXIS_ANGLE         180U

typedef enum ServoAxis{
    X_AXIS = 0,
    Y_AXIS = 1
}ServoAxis;

#define PACKET_BYTE_LENGTH      20U
#define SERIAL_BAUDRATE         115200U
const char16_t PACKET_START_MARKER = 0x6752;
const char PACKET_END_MARKER = 0xED;

void sendData(int &latitude,int &longitude,int &altitude,int &speed, int &heading);
void int32ToChar16String(uint32_t number,char16_t *output,size_t outputSize);

#ifdef __cplusplus
}
#endif

#endif
