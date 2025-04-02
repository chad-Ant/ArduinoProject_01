#include "../include/SignalProcessingFunctions.h"
#include "../include/MathFunctions.h"

PIDControls::PIDControls(float gainP, float integralTime, float derivativeTime, float integralLim, float upperBound, float lowerBound):
    Kp(gainP),
    setpoint(0.0f),
    Ti(integralTime),
    Td(derivativeTime),
    iLim(integralLim),
    prevInput(0.0f),
    uBound(upperBound),
    lBound(lowerBound),
    integral(0.0f),
    prevError(0.0f){
        Ki = integralTime == 0.0f ? 0.0f : gainP / divThreshold(integralTime);
        Kd = derivativeTime == 0.0f ? 0.0f : gainP / divThreshold(derivativeTime);
    }

bool PIDControls::calculate(float input, float &output, float setpoint, float dt_sec){
    if (dt_sec == 0.0f) return false;
    float error = setpoint - input;
    integral += error * dt_sec;
    saturate(integral, iLim, -iLim); //anti windup
    float derivative = (error - prevError) * fastReciprocal(dt_sec);
    prevError = error;
    output = error * Kp + integral * Ki + derivative * Kd;
    saturate(output, uBound, lBound);
    return true;
}

void PIDControls::setKp(float newKp){
    Kp = newKp;
    Ki = newKp / divThreshold(Ti);
    Kd = newKp / divThreshold(Td);
}

void PIDControls::setTi(float newTi){
    Ki = Kp / divThreshold(newTi);
}
void PIDControls::setTd(float newTd){
    Kd = Kp / divThreshold(newTd);
}