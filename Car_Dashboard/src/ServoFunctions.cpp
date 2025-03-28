#include "../include/ServoFunctions.h"

ServoReturnStatus initializeServo(Servo &servo_x, const int pin, int min, int max){
    servo_x.attach(pin, min, max);
    return servo_x.attached() ? SERVO_ATTACHED : SERVO_ERROR;
}

ServoReturnStatus readPosition(Servo &servo_x, int &position){
    if(!servo_x.attached()) return SERVO_NOT_ATTACHED;
    position = servo_x.read();
    return SERVO_COMMAND_SUCCESS;
}

ServoReturnStatus writePosition(Servo &servo_x, int position, int min, int max){
    if(!servo_x.attached()) return SERVO_NOT_ATTACHED;
    position = position >= min ? (position <= max ? position : max) : min;
    servo_x.write(position);
    return SERVO_COMMAND_SUCCESS;
}

ServoReturnStatus closeServo(Servo &servo_x){
    servo_x.detach();
    return !servo_x.attached() ? SERVO_DETACHED : SERVO_ERROR;
}