#ifndef DATA_DICTIONARY
#define DATA_DICTIONARY 1

#define GPS_PIN                                     7U
#define SERVO_XAXIS_PIN                             20U
#define SERVO_YAXIS_PIN                             19U
#define SEGLED_ADDRESS                              112U

#define SERIAL_BAUDRATE                             115200U
#define STATUS_INDICATOR                            LED_BUILTIN

constexpr char WIFI_SSID[] =                        "";
constexpr char WIFI_PASS[] =                        "";
unsigned constexpr long WIFI_TIMEOUT_MSEC =         10000;
constexpr int URL_BUFFER_SIZE =                     256;

unsigned constexpr long GPSBaudrate_Default =       9600;
unsigned constexpr long GPSBaudrate_Custom =        38400;
constexpr char AssistNowToken[] =                   "";
constexpr char AssistNowServer1[] =                 "https://online-live1.services.u-blox.com";
constexpr char AssistNowServer2[] =                 "https://online-live2.services.u-blox.com/GetOnlineData.ashx?token=%s;gnss=gps,glo;datatype=eph,alm,aux;";
constexpr char GETRequest_Online[] =                "/GetOnlineData.ashx?token=%s;gnss=gps,glo;datatype=eph,alm,aux;";
constexpr char GETRequest_Offline[] =               "https://offline-live1.services.u-blox.com/GetOfflineData.ashx?token=%s;gnss=gps,glo;period=1;resolution=1";

constexpr int MAX_XAXIS_ANGLE =                     180;
constexpr int MAX_YAXIS_ANGLE =                     180;

#endif