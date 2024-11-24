#ifndef SERVO_FUNCTIONS
#define SERVO_FUNCTIONS

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

#define SERVO_XAXIS_PWMIN       1
#define SERVO_XAXIS_PWMAX       2
#define SERVO_YAXIS_PWMIN       1
#define SERVO_YAXIS_PWMAX       2

bool initializeServo(Servo &servo_XAxis,Servo &servo_YAxis);
int readPosition(Servo &servo_XYAxis);
void writePosition(Servo &servo_XAxis,byte x,Servo &servo_YAxis,byte y);
void closeServo(Servo &servo_XAxis,Servo &servo_YAxis);

#endif