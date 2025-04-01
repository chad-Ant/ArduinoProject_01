#ifndef MATH_FUNCTIONS
#define MATH_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

//only use when absolutely necessary, otherwise use / as usual
float fastReciprocal(float num);
float interpolate(float num, float uBound, float lBound, float resultuBound = 100.0, float resultlBound = 0.0);

inline void rateLimit(float input, float &output, float delta){
    float tempNumUp = output + delta;
    float tempNumDown = output - delta;
    output = (input > tempNumUp) ? tempNumUp : ((input < tempNumDown) ? tempNumDown : input);
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