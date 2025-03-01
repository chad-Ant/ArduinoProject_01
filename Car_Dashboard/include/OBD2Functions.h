#ifndef OBD2FUNCTIONS_H
#define OBD2FUNCTIONS_H 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

enum OBD2Command{
    RPM_STD =       0x02010C,
    RPM_EXT =       0x02010C0000000000,
    SPEED_STD =     0x02010D,
    SPEED_EXT =     0x02010D0000000000
};

#endif