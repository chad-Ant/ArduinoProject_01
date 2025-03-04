#include "../include/OBD2Functions.h"

bool initializeOBD2(const OBD2Config config){
    if (!CAN.begin(config.CANBaudrate)){
        return false;
    }
    CAN.filter(config.RxAddress);
    return true;    
}

bool getSupportedPIDs(OBD2Config &config,time_t timeout){
    if (!CAN){
        return false;
    }
    static char supportedPIDs[4] = {0x00,0x00,0x00,0x00};

    for (int i = 0x00; i < 0xE0; i += 0x20){
        CAN.beginPacket(config.TxAddress,8);
        CAN.write(0x02);
        CAN.write(0x01);
        CAN.write(i);
        CAN.endPacket();

        while (CAN.parsePacket() == 0) {
            //todo: add timeout here, return false if timeout
            if (CAN.read() < 4) continue;
            if (CAN.read() != 0x41) continue;
            if (CAN.read() != i) continue;
            //wait for response
        }

        supportedPIDs[0] = CAN.read();
        supportedPIDs[1] = CAN.read();
        supportedPIDs[2] = CAN.read();
        supportedPIDs[3] = CAN.read();
    }
    return true;
}

bool sendData(OBD2Command command){
    if (!CAN){
        return false;
    }
    CAN.write();
}