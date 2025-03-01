#include "../include/OBD2Functions.h"

bool initializeOBD2(){
    return CAN.begin(CANBaudrate_Default);
}

bool sendData(OBD2Command command){
    if (!CAN){
        return false;
    }

}