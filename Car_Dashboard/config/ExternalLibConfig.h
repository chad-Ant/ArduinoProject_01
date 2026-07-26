#ifndef EXTERNAL_LIB_CONFIG
#define EXTERNAL_LIB_CONFIG 1

#include <Wire.h>
// GPS libraries (SparkFun u-blox GNSS) are owned by GPSFunctions.h/.cpp, not bundled here.
// Servo library is owned by ServoFunctions.h/.cpp, not bundled here.
#include <Arduino.h>
// LED backpack (Adafruit_AlphaNum4) is owned by SegmentLEDFunctions.h/.cpp, not bundled here.
//#include "LowPower.h"
#include <SPI.h>
// WiFi stack (WiFi101 / ArduinoHttpClient / WiFiUdp) is owned by
// WiFiFunctions.h, HTTPClientFunctions.h and UDPClientFunctions.h — not bundled here.
// CAN library (arduino-CAN) is owned by OBD2Functions.h/.cpp, not bundled here.
// SD libraries (SdFat) are owned by SDFunctions.h/.cpp, not bundled here.
#include <TimeLib.h>
#include <RTCZero.h>
#include <cmath>
#include <memory>
#include <climits>
#include "time.h"

// swapCustom() (the std::swap substitute) now lives in include/MathFunctions.h,
// alongside the interpolate()/saturate() helpers that use it.

#endif