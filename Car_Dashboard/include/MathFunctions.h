#ifndef MATH_FUNCTIONS
#define MATH_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

enum FilterWSize{
    SIZE_8 = 8,
    SIZE_16 = 16,
    SIZE_32 = 32
};

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
        SimpleMovingAverage& operator=(const SimpleMovingAverage&) = delete;
        bool calculate(float &input,float &output);
        void reset();
};

#endif