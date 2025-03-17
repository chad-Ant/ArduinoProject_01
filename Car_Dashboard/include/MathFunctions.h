#ifndef MATH_FUNCTIONS
#define MATH_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

bool splitByte(const char* input, char* outputBuffer,const size_t outputBufferLength, size_t byteLength, size_t byteOffset);

class MovingAverage{
    private:
        size_t windowSize;
        float* outputBuffer;
        uint8_t bufferIndex;
        unsigned int currentDivisor;
        float sum;

    public:
        MovingAverage(const uint32_t wSize);
        ~MovingAverage();

        MovingAverage(const MovingAverage&) = delete;
        MovingAverage& operator=(const MovingAverage&) = delete;
        void execute(float &input,float &output, bool reset = false);
}

#endif