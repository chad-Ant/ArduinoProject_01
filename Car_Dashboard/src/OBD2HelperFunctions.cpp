#include "../include/OBD2Functions.h"

bool initializeOBD2(){
    if (!CAN.begin(CANBaudrate_Default)){
        return false;
    }
#ifdef CAN_STD_ADDRESSING
    CAN.filter(0x7E8);
#endif
#ifdef CAN_EXT_ADDRESSING
    CAN.filter(0x18DAF110);
#endif
    return true;
}
bool getSupportedPIDs(OBD2Command pids){
#ifdef CAN_STD_ADDRESSING
    CAN.beginPacket(0x7DF,8);
#endif
#ifdef CAN_EXT_ADDRESSING
    CAN.beginExtendedPacket(0x18DB33F1, 8);
#endif
    CAN.write();

}
bool sendData(OBD2Command command){
    if (!CAN){
        return false;
    }
    CAN.write();
}