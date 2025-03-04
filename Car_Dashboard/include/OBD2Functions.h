#ifndef OBD2FUNCTIONS_H
#define OBD2FUNCTIONS_H 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

//https://en.wikipedia.org/wiki/OBD-II_PIDs#Query

#ifdef CAN_STD_ADDRESSING
enum OBD2Command{
    RPM =           0x02010C,
    SPEED =         0x02010D,
    GEAR_RATIO =    0x0201A4,
    ABS_PRESSURE =  0x020133
};
#endif
#ifdef CAN_EXT_ADDRESSING
enum OBD2Command{
    RPM =           0x02010C0000000000,
    SPEED =         0x02010D0000000000,
    GEAR_RATIO =    0x0201A40000000000,
    ABS_PRESSURE =  0x0201330000000000
};
#endif

#endif