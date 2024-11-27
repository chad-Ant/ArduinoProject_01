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
constexpr char AssistNowToken[] =                   "x-xF25aCRFyCVqidX0-3Pw";
constexpr char AssistNowServer1[] =                 "online-live1.services.u-blox.com";
constexpr char AssistNowServer2[] =                 "online-live2.services.u-blox.com";
constexpr char GETRequest_Online[] =                "/GetOnlineData.ashx?token=%s;gnss=gps,glo;datatype=eph,alm,aux;";
constexpr char GETRequest_Offline[] =               "/GetOfflineData.ashx?token=%s;gnss=gps,glo;period=1;resolution=1";
#define USE_DEFAULT_LOCATION 1
#ifdef USE_HOCHIMINH    //10.81532915851147, 106.6573371901137
constexpr char cacheLat[] = "lat=10.81532915851147";
constexpr char cacheLong[] = "lon=106.6573371901137";
constexpr char cacheAlt[] = "alt=7";
constexpr char cacheAcc[] = "pacc=50000";
#endif
#ifdef USE_VUNGTAU      //10.367506732991703, 107.08795322622392
constexpr char cacheLat[] = "lat=10.367506732991703";
constexpr char cacheLong[] = "lon=107.08795322622392";
constexpr char cacheAlt[] = "alt=7";
constexpr char cacheAcc[] = "pacc=50000";
#endif
#ifdef USE_DEFAULT_LOCATION //10.81532915851147, 106.6573371901137, 100km radius
constexpr char cacheLat[] = "lat=10.81532915851147";
constexpr char cacheLong[] = "lon=106.6573371901137";
constexpr char cacheAlt[] = "alt=7";
constexpr char cacheAcc[] = "pacc=100000";
#endif

constexpr int MAX_XAXIS_ANGLE =                     180;
constexpr int MAX_YAXIS_ANGLE =                     180;

#endif