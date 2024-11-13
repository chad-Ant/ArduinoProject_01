#include "..\include\HelperFunctions.h"
#include <Arduino.h>
#include <Servo.h>

Servo servo_XAxis;
Servo servo_YAxis;

void beginServo(){
    servo_XAxis.attach(SERVO_XAXIS_PIN);
    servo_YAxis.attach(SERVO_YAXIS_PIN);
}

int readPosition(ServoAxis axis){
   if (axis == X_AXIS){
    return servo_XAxis.read();
   }
   if (axis == Y_AXIS){
    return servo_XAxis.read();
   }
   return 0xFFFFFFFF;
}

void writePosition(byte x, byte y){
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

void endServo(){
    
}