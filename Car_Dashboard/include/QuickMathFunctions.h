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

#endif