#ifndef COMMUNICATION_FUNCTIONS
#define COMMUNICATION_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

bool splitByte(const char* input, char* outputBuffer,const size_t outputBufferLength, size_t byteLength, size_t byteOffset);

#endif