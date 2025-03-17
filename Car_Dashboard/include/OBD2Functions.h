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
    NONE =          0x00,
    RPM =           0x0C,
    SPEED =         0x0D,
    GEAR_RTIO =    0xA4,
    AIR_PRES =      0x33,
    ODOMETER =      0xA6,
    FUEL_LVL =      0x2F,
    ENGINE_TEMP =    0x05,
    FUEL_RATE =     0x5E,
    ENGINE_LOAD =   0x04,
    THROTTLE_POSN =  0x11
};

struct OBD2Config{
    uint32_t CANBaudrate;
    CAN_TxAddress TxAddress;
    CAN_RxAddress RxAddress;
    uint32_t supportedPIDs[7];
};

bool initializeOBD2(const OBD2Config config);
bool checkCANModule();
bool getSupportedPIDs(OBD2Config &config,long timeoutInterval);
bool sendS1Command(OBD2Config &config, const OBD2_S1Command command);


const SPISettings SPICfg(10E6, MSBFIRST, SPI_MODE0);

#endif