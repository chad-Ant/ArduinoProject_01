#ifndef DATA_DICTIONARY
#define DATA_DICTIONARY 1

#define GPS_PIN                             7U
#define SERVO_XAXIS_PIN                     20U
#define SERVO_YAXIS_PIN                     19U
#define SEGLED_ADDRESS                      0x70

#define SERIAL_BAUDRATE                     115200U
#define STATUS_INDICATOR                    LED_BUILTIN

const char WIFI_SSID[] =                    "";
const char WIFI_PASS[] =                    "";
unsigned const long WIFI_TIMEOUT_MSEC =     10000;

unsigned const long GPSBaudrate_Default =   9600;
unsigned const long GPSBaudrate_Custom =    38400;
const char AssistNowToken[] =               "";
const char AssistNowServer1[] =             "online-live1.services.u-blox.com";
const char AssistNowServer2[] =             "online-live2.services.u-blox.com";

const int MAX_XAXIS_ANGLE =                 180;
const int MAX_YAXIS_ANGLE =                 180;

#endif