#ifndef SIGPROC_FUNCTIONS
#define SIGPROC_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../include/MathFunctions.h"

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
        float setPoint;
        float gainP;
        float gainI;
        float gainD;
        float setpoint;
    public:
        PIDControls();
        ~PIDControls();

        PIDControls(const PIDControls&) = delete;
        PIDControls& operator = (const PIDControls&) = delete;

        bool setModel(float *model);

};

#endif