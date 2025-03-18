#include "../include/MathFunctions.h"

bool splitByte(const char* input, char* outputBuffer,const size_t outputBufferLength, size_t byteLength, size_t byteOffset){
    if (!input || !outputBuffer){
        return false;
    }
    
    if (byteOffset >= strlen(input)){
        return false;
    }
    
    if (byteLength == 0 || outputBufferLength == 0){
        return false;
    }

    if (byteLength + byteOffset > strlen(input)){
        byteLength = strlen(input) - byteOffset;
    }

    memset(outputBuffer,0,outputBufferLength); //clear the output buffer
    size_t lenToCopy = (byteLength < outputBufferLength) ? byteLength : outputBufferLength;
    memcpy(outputBuffer, input+byteOffset, lenToCopy);
    return true;
}

MovingAverage::MovingAverage(const uint32_t wSize):
    windowSize(wSize < 1 ? 1 : wSize),
    outputBuffer(new float[wSize < 1 ? 1 : wSize]),
    bufferIndex(0),
    currentDivisor(1),
    sum(0){
        memset(outputBuffer, 0, windowSize * sizeof(float)); //can get away with memset 0 because arduino uses IEEE 754
    }

MovingAverage::~MovingAverage(){
    delete[] outputBuffer;
}

void MovingAverage::execute(float &input,float &output, bool reset){  
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