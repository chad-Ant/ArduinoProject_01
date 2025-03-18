#ifndef QUICK_MATH_FUNCTIONS
#define QUICK_MATH_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"

inline int32_t div10Approx(int32_t number){
    /*!Will not check for overflow
    not accurate for large number
    */
    return (6554 * number) >> 16;
}

inline int32_t div100Approx(int32_t number){
    /*!Will not check for overflow
    not accurate for large number
    */
    return (10486 * number) >> 20;
}

inline int64_t div1000Approx(int64_t number){
    /*!Will not check for overflow
    not accurate for large number
    */
   return (536871 * number) >> 29;
}

inline uint8_t counterU8(bool reset = false){
    static uint8_t counter = 0;
    if (reset){
        counter = 0;
    }
    return counter++;
}

inline uint16_t counterU16(bool reset = false){
    static uint16_t counter = 0;
    if (reset){
        counter = 0;
    }
    return counter++;
}

inline uint32_t counterU32(bool reset = false){
    static uint32_t counter = 0;
    if (reset){
        counter = 0;
    }
    return counter++;
}

inline uint64_t counterU64(bool reset = false){
    static uint64_t counter = 0;
    if (reset){
        counter = 0;
    }
    return counter++;
}

#endif