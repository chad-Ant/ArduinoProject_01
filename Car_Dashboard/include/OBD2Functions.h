#ifndef OBD2FUNCTIONS_H
#define OBD2FUNCTIONS_H 1

#include <cstdint>
#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

enum CAN_TxAddress{
    OBD2_TX_GLOBAL =        0x7DF,
    OBD2_TX_ECM_1 =         0x7E0,
};
enum CAN_RxAddress{
    OBD2_RX_ECM_1 =         0x7E8
};

//https://en.wikipedia.org/wiki/OBD-II_PIDs#Query

enum OBD2_S1Command{
    NONE =          0x000000,
    RPM =           0x02010C,
    SPEED =         0x02010D,
    GEAR_RTIO =    0x0201A4,
    AIR_PRES =      0x020133,
    ODOMETER =      0x0201A6,
    FUEL_LVL =      0x02012F,
    ENGINE_TEMP =    0x020105,
    FUEL_RATE =     0x02015E,
    ENGINE_LOAD =   0x020104,
    THROTTLE_POSN =  0x020111
};

struct OBD2Config{
    uint32_t CANBaudrate;
    CAN_TxAddress TxAddress;
    CAN_RxAddress RxAddress;
    OBD2_S1Command supportedPIDs[16];
};

#endif