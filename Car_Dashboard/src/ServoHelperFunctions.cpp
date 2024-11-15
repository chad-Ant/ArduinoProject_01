#include "..\include\HelperFunctions.h"
#include <Arduino.h>
#include <Servo.h>

//Servo servo_XAxis;
//Servo servo_YAxis;

bool initializeServo(Servo &servo_XAxis,Servo &servo_YAxis){
    servo_XAxis.attach(SERVO_XAXIS_PIN,SERVO_XAXIS_PWMIN,SERVO_XAXIS_PWMAX);
    servo_YAxis.attach(SERVO_YAXIS_PIN,SERVO_YAXIS_PWMIN,SERVO_YAXIS_PWMAX);
    return (servo_XAxis.attached() && servo_YAxis.attached());
}

int readPosition(Servo &servo_XYAxis){
    return servo_XYAxis.read();
}

void writePosition(Servo &servo_XAxis,byte x,Servo &servo_YAxis,byte y){
    if (x < 0){
        x = 0;
    }
    else if (x > MAX_XAXIS_ANGLE){
        x = MAX_XAXIS_ANGLE;
    }

    if (y < 0){
        y = 0;
    }
    else if (y > MAX_YAXIS_ANGLE){
        y = MAX_YAXIS_ANGLE;
    }

    servo_XAxis.write(x);
    servo_YAxis.write(y);
}

void closeServo(Servo &servo_XAxis,Servo &servo_YAxis){
    servo_XAxis.detach();
    servo_YAxis.detach();
}