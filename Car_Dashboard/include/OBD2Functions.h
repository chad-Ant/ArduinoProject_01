#ifndef OBD2FUNCTIONS_H
#define OBD2FUNCTIONS_H 1

#include <cstdint>
#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"
#define OBD2_TIMEOUT_MSEC 10000UL
//CAN module uses SPI 10E6, MSBFIRST, SPI_MODE0 CS pin 3, INT pin 7
const SPISettings SPICfg(10E6, MSBFIRST, SPI_MODE0);

enum class CANReturnStatus{
    OK = 0,
    BAD_CFG = 1,
    NOK_INIT_FAILED = -1,
    NOK_STATUS_BAD = -2,
    NOK_BAD_CMD = -3,
    NOK_NULL_BUFFER = -4,
    NOK_NOT_S1_CFG = -5,
    NOK_TIMEOUT = -6
};

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
    CAN_TxAddress TxAddress;
    CAN_RxAddress RxAddress;
    uint32_t supportedPIDs[7];
};

CANReturnStatus initializeOBD2(OBD2Config &config, CAN_TxAddress TxAddr, CAN_RxAddress RxAddr);
bool checkCANModule();
CANReturnStatus getSupportedPIDs(OBD2Config &config,long timeoutInterval = OBD2_TIMEOUT_MSEC);
CANReturnStatus sendS1Command(OBD2Config &config, const OBD2_S1Command command);
CANReturnStatus receiveS1Command(OBD2Config &config, char *outputBuffer, unsigned long timeout, OBD2_S1Command &commandRx);
#endif