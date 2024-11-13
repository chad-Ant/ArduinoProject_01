#include "..\include\HelperFunctions.h"
#include <Arduino.h>


void sendData(int latitude,int longitude,int altitude,int speed, int heading){
}

void int32ToChar16String(uint32_t number,char16_t *output,size_t outputSize){
    if (outputSize == 0 || output == nullptr) return;
    size_t index = 0;

    if (number < 0) {
        if (index < outputSize - 1) {
            output[index++] = u'-';
        } else {
            // Not enough space to store the minus sign
            output[0] = u'\0';
            return;
        }
        number = -number;
    }

    char16_t tempBuffer[12]; // Buffer to hold digits in reverse
    size_t tempIndex = 0;

    // Handle zero explicitly
    if (number == 0) {
        tempBuffer[tempIndex++] = u'0';
    } else {
        while (number > 0 && tempIndex < sizeof(tempBuffer)/sizeof(tempBuffer[0])) {
            int digit = number % 10;
            tempBuffer[tempIndex++] = u'0' + digit;
            number /= 10;
        }
    }

    while (tempIndex > 0 && index < outputSize - 1) {
        output[index++] = tempBuffer[--tempIndex];
    }

    output[index] = u'\0'; // Null-terminate the string
}
