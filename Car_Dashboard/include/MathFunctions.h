#ifndef MATH_FUNCTIONS
#define MATH_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

bool splitByte(const char* input, char* outputBuffer,const size_t outputBufferLength, size_t byteLength, size_t byteOffset);

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

        static constexpr bool isPowerOfTwo(FilterWSize size) {
            return size && !(size & (size - 1));
        }
        static_assert(isPowerOfTwo(SIZE_8) && isPowerOfTwo(SIZE_16) && isPowerOfTwo(SIZE_32),
                      "Window sizes must be powers of 2");

    public:
        SimpleMovingAverage(const FilterWSize wSize);
        ~SimpleMovingAverage();

        SimpleMovingAverage(const SimpleMovingAverage&) = delete;
        SimpleMovingAverage& operator=(const SimpleMovingAverage&) = delete;
        bool calculate(float &input,float &output);
        void reset();
};

#endif