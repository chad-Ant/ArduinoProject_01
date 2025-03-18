#ifndef SERVO_FUNCTIONS
#define SERVO_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

bool initializeServo(Servo &servo_x, const int pin, int min = 0, int max = MAX_XAXIS_ANGLE);
int readPosition(Servo &servo_x);
void writePosition(Servo &servo_x, int position, const int min = 0, const int max = MAX_XAXIS_ANGLE);
bool closeServo(Servo &servo_x);

#endif