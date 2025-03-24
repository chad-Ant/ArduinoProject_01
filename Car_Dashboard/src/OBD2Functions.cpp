#include "../include/OBD2Functions.h"
#include "../include/TimerFunctions.h"

void writeRegister(uint8_t address, uint8_t value)
{
  SPI.beginTransaction(SPICfg);
  digitalWrite(MCP2515_DEFAULT_CS_PIN, LOW);
  SPI.transfer(0x02);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(MCP2515_DEFAULT_CS_PIN, HIGH);
  SPI.endTransaction();
}

uint8_t readRegister(uint8_t address)
{
  uint8_t value;

  SPI.beginTransaction(SPICfg);
  digitalWrite(MCP2515_DEFAULT_CS_PIN, LOW);
  SPI.transfer(0x03);
  SPI.transfer(address);
  value = SPI.transfer(0x00);
  digitalWrite(MCP2515_DEFAULT_CS_PIN, HIGH);
  SPI.endTransaction();

  return value;
}

bool initializeOBD2(const OBD2Config config){
    if (!CAN.begin(config.CANBaudrate)){
        return false;
    }
    CAN.filter(config.RxAddress);
    return true;    
}

bool checkCANModule(){
    writeRegister(0x0f, 0x80);          //0x0f = REG_CANCTRL
    if (readRegister(0x0f) != 0x80){
        return false;
    }
    return true;
}

bool getSupportedPIDs(OBD2Config &config,long timeoutInterval){
    if (!checkCANModule()){
        return false;
    }
    uint32_t PIDs = 0;
    uint8_t tempPID = 0x00;
    for (uint8_t i = 0x00; i < sizeof(config.supportedPIDs); i++){
        CAN.beginPacket(config.TxAddress,8);
        CAN.write(0x02);
        CAN.write(0x01);
        CAN.write(tempPID);
        CAN.endPacket();
        
        unsigned long thisRun = millis();
        while (CAN.parsePacket() == 0 || !isTimeout(timeoutInterval,thisRun)){ {
            if (CAN.read() < 6) continue;
            if (CAN.read() != 0x41) continue;
            if (CAN.read() != tempPID) continue;
            delay(50);
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
}

bool isCommandSupported(OBD2Config &config, const OBD2_S1Command command){
    int PIDGroup = command / 0x20;
    int PIDIndex = command % 0x20;
    
    if (PIDGroup >= 7){
        return false;
    }

    return (config.supportedPIDs[PIDGroup] >> (31 - PIDIndex) & 1)== 1;
}

bool sendS1Command(OBD2Config &config, const OBD2_S1Command command){
    if (command == NONE){
        return false;
    }
    
    CAN.beginPacket(config.TxAddress, 8);
    CAN.write(0x02);    //additional bytes in Tx packet
    CAN.write(0x01);    //current data
    CAN.write(command);
    CAN.endPacket();

    return true;
}

bool receiveS1Command(OBD2Config &config, const char *outputBuffer, unsigned long timeout, OBD2_S1Command &commandRx){
    /* todo: fix bug line 112, 119
    if (!outputBuffer){
        return false;
    }

    bool packetParsed = CAN.parsePacket() != 0;
    uint8_t RxLength = CAN.read() - 2;
    bool isS1Response = CAN.read() == 0x41;
    commandRx = CAN.read();

    if (!packetParsed || RxLength < 1 || !isS1Response){
        return false;
    }

    for (int i = 0; i < RxLength; i++){
        outputBuffer[i] = CAN.read();
    }
    return true;
    */
}