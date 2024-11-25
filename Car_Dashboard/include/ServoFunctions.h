#ifndef SERVO_FUNCTIONS
#define SERVO_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

constexpr int8_t SERVO_XAXIS_PWMIN =    1;
constexpr int8_t SERVO_XAXIS_PWMAX =    2;
constexpr int8_t SERVO_YAXIS_PWMIN =    1;
constexpr int8_t SERVO_YAXIS_PWMAX =    2;

bool initializeServo(Servo &servo_XAxis,Servo &servo_YAxis);
int readPosition(Servo &servo_XYAxis);
void writePosition(Servo &servo_XAxis,byte x,Servo &servo_YAxis,byte y);
void closeServo(Servo &servo_XAxis,Servo &servo_YAxis);

#endif