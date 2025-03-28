#ifndef SERVO_FUNCTIONS
#define SERVO_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

enum ServoReturnStatus{
    SERVO_ATTACHED = 0,
    SERVO_DETACHED = 1,
    SERVO_COMMAND_SUCCESS = 2,
    SERVO_ERROR = -1,
    SERVO_NOT_ATTACHED = -2,
    SERVO_NOT_DETACHED = -3
};

ServoReturnStatus initializeServo(Servo &servo_x, const int pin, int min = 0, int max = MAX_XAXIS_ANGLE);
ServoReturnStatus readPosition(Servo &servo_x, int &position);
ServoReturnStatus writePosition(Servo &servo_x, int position, int min = 0, int max = MAX_XAXIS_ANGLE);
ServoReturnStatus closeServo(Servo &servo_x);

#endif