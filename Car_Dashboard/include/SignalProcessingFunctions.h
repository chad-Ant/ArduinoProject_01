#ifndef SIGPROC_FUNCTIONS
#define SIGPROC_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"

enum FilterWSize{
    SIZE_8 = 8,
    SIZE_16 = 16,
    SIZE_32 = 32
};

inline void counterU8(uint8_t &counter, bool reset = false){
    if (reset){
        counter = 0;
    }
    counter++;
}

inline void counterU16(uint16_t &counter, bool reset = false){
    if (reset){
        counter = 0;
    }
    counter++;
}

inline void counterU32(uint32_t &counter, bool reset = false){
    if (reset){
        counter = 0;
    }
    counter++;
}

inline void saturate(float &num, float uBound, float lBound){
    if (uBound < lBound) swapCustom(uBound,lBound);
    num = num < uBound ? (num > lBound ? num : lBound) : uBound;
}

class SimpleMovingAverage{
    private:
        FilterWSize windowSize;
        float* outputBuffer;
        uint8_t bufferIndex;
        float reciprocalDivisor;
        float sum;
        bool outputValid;

    public:
        SimpleMovingAverage(const FilterWSize wSize);
        ~SimpleMovingAverage();

        SimpleMovingAverage(const SimpleMovingAverage&) = delete;
        SimpleMovingAverage& operator = (const SimpleMovingAverage&) = delete;

        bool calculate(float &input,float &output);
        void reset();
};

class PIDControls{
    private:
        float integral;
        float prevError;
        float Kp;
        float Ki;
        float Kd;
        float iLim;
        float prevInput;
    public:
        float Ti;
        float Td;
        float setpoint;
        float uBound;
        float lBound;

        PIDControls(float gainP, float integralTime, float derivativeTime, float integralLim, float upperBound, float lowerBound);
        ~PIDControls();

        PIDControls(const PIDControls&) = delete;
        PIDControls& operator = (const PIDControls&) = delete;
        
        bool calculate(float input, float &output, float setpoint, float dt_sec);
        void setKp(float newKp);
        void setTi(float newTi);
        void setTd(float newTd);
};

#endif