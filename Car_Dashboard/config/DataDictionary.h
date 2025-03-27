#ifndef DATA_DICTIONARY
#define DATA_DICTIONARY 1

#define GPS_PIN                                     7U
#define SERVO_XAXIS_PIN                             20U
#define SERVO_YAXIS_PIN                             19U
#define SEGLED_ADDRESS                              112U

#define SERIAL_BAUDRATE                             115200U
#define STATUS_INDICATOR                            LED_BUILTIN

#define WIFI_SSID                                   "Pixel_5101"
#define WIFI_PASS                                   "AndroidPixel6"
#define WIFI_SSID_BACKUP_1                          "TP-Link_2890"
#define WIFI_PASS_BACKUP_1                          "52000393"
#define WIFI_SSID_BACKUP_2                          "yyy"
#define WIFI_PASS_BACKUP_2                          "yyy"
#define WIFI_SSID_BACKUP_3                          "zzz"
#define WIFI_PASS_BACKUP_3                          "zzz"
#define URL_BUFFER_SIZE                             256

#define GPS_REFRESH_RATE                            4U
#define GPSBaudrate_Default                         9600U
#define GPSBaudrate_Custom                          115200UL
#define AssistNowToken                              "x-xF25aCRFyCVqidX0-3Pw" //add token before compiling 
#define AssistNowServer1                            "online-live1.services.u-blox.com"
#define AssistNowServer2                            "online-live2.services.u-blox.com"
//#define AssistNowServer1_OFFLINE                    "offline-live1.services.u-blox.com"
//#define AssistNowServer2_OFFLINE                    "offline-live2.services.u-blox.com"
#define GETRequest_Online                           "/GetOnlineData.ashx?token=%s;gnss=gps,glo,qzss,gal;datatype=eph,alm,aux,pos;%s"
//#define GETRequest_Offline                          "/GetOfflineData.ashx?token=%s;gnss=gps,glo,gal;period=1;resolution=1"
///GetOnlineData.ashx?token=;gnss=gps,glo,qzss,gal;datatype=eph,alm,aux,pos;lat=10.815329;lon=106.657337;alt=10.000000;pacc=10000.000000
#define USE_DEFAULT_LOCATION 1
#ifdef USE_HOCHIMINH    //10.81532915851147, 106.6573371901137
#define cachePos                                    "lat=10.81532915851147;lon=106.6573371901137;alt=10.000000;pacc=50000.000000"
#endif
#ifdef USE_VUNGTAU      //10.367506732991703, 107.08795322622392
#define cachePos                                    "lat=10.367506732991703;lon=107.08795322622392;alt=7.000000;pacc=50000.000000"
#endif
#ifdef USE_DEFAULT_LOCATION //10.81532915851147, 106.6573371901137, 100km radius
#define cachePos                                    "lat=10.81532915851147;lon=106.6573371901137;alt=10.000000;pacc=100000.000000"
#endif
#define LOCAL_TIMEZONE                              7

#define CANBaudrate_Default                         500000UL
//SPI interface pins, avoid using these for other purposes
//#define MCP2515_DEFAULT_CS_PIN          3
//#define MCP2515_DEFAULT_INT_PIN         7


#define MAX_XAXIS_ANGLE                             180
#define MAX_YAXIS_ANGLE                             180

#endif