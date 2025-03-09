#include "../include/OBD2Functions.h"
#include "../include/TimerFunctions.h"

bool initializeOBD2(const OBD2Config config){
    if (!CAN.begin(config.CANBaudrate)){
        return false;
    }
    CAN.filter(config.RxAddress);
    return true;    
}

bool getSupportedPIDs(OBD2Config &config,long timeoutInterval){
    if (!CAN){
        return false;
    }
    long PIDs = 0;
    uint8_t tempPID = 0x00;
    for (uint8_t i = 0x00; i < sizeof(config.supportedPIDs); i++){
        CAN.beginPacket(config.TxAddress,8);
        CAN.write(0x02);
        CAN.write(0x01);
        CAN.write(tempPID);
        CAN.endPacket();
        
        unsigned long lastRun = millis();
        while (CAN.parsePacket() == 0 || !timeout(timeoutInterval,lastRun)){ {
            if (CAN.read() < 6) continue;
            if (CAN.read() != 0x41) continue;
            if (CAN.read() != tempPID) continue;
            //wait for response
        }

        for (int j = 0; j < 4; j++){
            PIDs <<=8;
            PIDs |= CAN.read();
        }
        
        config.supportedPIDs[i] = PIDs;
        tempPID += 0x20;
    }
    return true;
}

bool sendData(OBD2Command command){
    if (!CAN){
        return false;
    }
    CAN.write();
}