#include "..\include\HelperFunctions.h"
#include <Arduino.h>
#include <cmath>

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

inline int32_t div1000Approx(int32_t number){
    /*!Will not check for overflow
    not accurate for large number
    */
   return (536871 * number) >> 29;
}