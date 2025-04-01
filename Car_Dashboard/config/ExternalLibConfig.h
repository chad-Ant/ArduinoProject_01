#ifndef EXTERNAL_LIB_CONFIG
#define EXTERNAL_LIB_CONFIG 1

#include <Wire.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <Servo.h>
#include <Arduino.h>
#include "Adafruit_LEDBackpack.h"
//#include "LowPower.h"
#include <SPI.h>
#include <WiFi101.h>
#include <ArduinoHttpClient.h>
#include <WiFiUdp.h>
#include <CAN.h>
#include <TimeLib.h>
#include <RTCZero.h>
#include <cmath>
#include <memory>
#include <climits>
#include "time.h"

//substitute for std::swap
template <typename T>
void swapCustom(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

#endif