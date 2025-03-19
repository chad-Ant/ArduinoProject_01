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