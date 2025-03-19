#include "../include/MathFunctions.h"

SimpleMovingAverage::SimpleMovingAverage(const uint32_t wSize):
    windowSize(wSize < 1 ? 1 : wSize),
    outputBuffer(new float[wSize < 1 ? 1 : wSize]),
    bufferIndex(0),
    currentDivisor(1),
    sum(0){
        memset(outputBuffer, 0, windowSize * sizeof(float)); //can get away with memset 0 because arduino uses IEEE 754
    }

SimpleMovingAverage::~SimpleMovingAverage(){
    delete[] outputBuffer;
}

void SimpleMovingAverage::execute(float &input,float &output, bool reset){  
    if (reset){
        memset(outputBuffer, 0, windowSize * sizeof(float));
        bufferIndex = 0;
        currentDivisor = 1;
        sum = 0;        
    }

    sum += input - outputBuffer[bufferIndex];
    outputBuffer[bufferIndex] = input;
    bufferIndex = (bufferIndex + 1) % windowSize;
    currentDivisor = currentDivisor < windowSize ? currentDivisor + 1 : windowSize; //warm-up handling
    output = sum / (float)(currentDivisor);
}