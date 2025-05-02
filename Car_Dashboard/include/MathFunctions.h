#ifndef MATH_FUNCTIONS
#define MATH_FUNCTIONS 1

#define DIVISOR_LIMIT 0.001F

#include "../config/ExternalLibConfig.h"

//only use when absolutely necessary, otherwise use / as usual
float fastReciprocal(float num);
float interpolate(float num, float uBound, float lBound, float resultuBound = 100.0, float resultlBound = 0.0);

inline float divThreshold(float num){
    return fabsf(num) < DIVISOR_LIMIT ? (num >= 0 ? DIVISOR_LIMIT : -DIVISOR_LIMIT) : num;
}

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

#endif