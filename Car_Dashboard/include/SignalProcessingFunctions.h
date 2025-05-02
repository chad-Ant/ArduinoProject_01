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

inline void rateLimit(float input, float &output, float delta){
    float tempNumUp = output + delta;
    float tempNumDown = output - delta;
    output = (input > tempNumUp) ? tempNumUp : ((input < tempNumDown) ? tempNumDown : input);
}

inline void saturate(float &num, float uBound, float lBound){
    if (uBound < lBound) swapCustom(uBound,lBound);
    num = num < uBound ? (num > lBound ? num : lBound) : uBound;
}

class SimpleMovingAverage{
    public:
        SimpleMovingAverage(const FilterWSize wSize);
        ~SimpleMovingAverage();

        SimpleMovingAverage(const SimpleMovingAverage&) = delete;
        SimpleMovingAverage& operator = (const SimpleMovingAverage&) = delete;

        bool calculate(float &input,float &output);
        void reset();
    private:
        FilterWSize windowSize;
        float* outputBuffer;
        uint8_t bufferIndex;
        float reciprocalDivisor;
        float sum;
        bool outputValid;

};

class PIDControls{
    public:
        float sp;
        float uBound;
        float lBound;
        enum state{ON, OFF};

        PIDControls(float gainP, float integralTime, float derivativeTime, float setpoint, float upperBound, float lowerBound);
        ~PIDControls();

        PIDControls(const PIDControls&) = delete;
        PIDControls& operator = (const PIDControls&) = delete;
        
        void reset(float input, float &output);
        bool calculate(float input, float &output, float dt_sec);
        void enable(float input, float output);
        void disable();
        bool getState();
        void setKp(float newKp, float &output);
        void setTi(float newTi, float &output);
        void setTd(float newTd, float &output);
        
    private:
        float Kp;
        float Ti;
        float Td;
        float Ki;
        float Kd;
        float prevInput;
        float prevError;
        float integral;
        float derivative;
        state controlState;
};

#endif