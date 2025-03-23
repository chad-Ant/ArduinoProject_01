#include "../include/ServoFunctions.h"

bool initializeServo(Servo &servo_x, const int pin, int min, int max){
    servo_x.attach(pin, min, max);
    return servo_x.attached();
}

int readPosition(Servo &servo_x){
    return servo_x.read();
}

void writePosition(Servo &servo_x, int position, int min, int max){
    position = position >= min ? (position <= max ? position : max) : min;
    servo_x.write(position);
}

bool closeServo(Servo &servo_x){
    servo_x.detach();
    return !servo_x.attached();
}