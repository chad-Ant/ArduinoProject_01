#include "../include/QuickMathFunctions.h"

void splitByte(const char* input, char* outputBuffer,const size_t outputBufferLength, size_t byteLength, size_t byteOffset){
    memset(outputBuffer,0,outputBufferLength); //clear the output buffer
    size_t lenToCopy = (byteLength < outputBufferLength) ? byteLength : outputBufferLength;
    memcpy(outputBuffer, input+byteOffset, lenToCopy);
}
