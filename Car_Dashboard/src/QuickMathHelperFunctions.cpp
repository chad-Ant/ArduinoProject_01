#include "../include/QuickMathFunctions.h"

void splitBit(const char* input, char* outputBuffer, size_t outputBufferLength, size_t bitLength, size_t bitOffset){
    memset(outputBuffer,0,outputBufferLength);
    size_t maxBitLength = outputBufferLength * 8;
    size_t bitsToCopy = (bitLength < maxBitLength) ? bitLength : maxBitLength;

}

void splitBit64(const uint64_t input, uint64_t* outputBuffer, size_t bitLength, size_t bitOffset){
    
}