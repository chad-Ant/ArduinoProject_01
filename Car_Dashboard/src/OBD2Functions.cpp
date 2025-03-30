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

bool checkCANModule(){
    writeRegister(0x0f, 0x80);          //0x0f = REG_CANCTRL
    if (readRegister(0x0f) != 0x80){
        return false;
    }
    return true;
}

CANReturnStatus getSupportedPIDs(OBD2Config &config,long timeoutInterval){
    if (!checkCANModule()){
        return CANReturnStatus::NOK_STATUS_BAD;
    }
    uint32_t PIDs = 0;
    uint8_t tempPID = 0x00;
    for (uint8_t i = 0x00; i < sizeof(config.supportedPIDs) / sizeof(config.supportedPIDs[0]); i++){
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
    return CANReturnStatus::OK;
    }
}

CANReturnStatus initializeOBD2(OBD2Config &config, CAN_TxAddress TxAddr, CAN_RxAddress RxAddr){
    if (!CAN.begin(500E3)){
        return CANReturnStatus::NOK_INIT_FAILED;
    }
    config.TxAddress = TxAddr;
    config.RxAddress = RxAddr;
    getSupportedPIDs(config);
    CAN.filter(config.RxAddress);
    return CANReturnStatus::OK;    
}

bool isCommandSupported(OBD2Config &config, const OBD2_S1Command command){
    int PIDGroup = command / 0x20;
    int PIDIndex = command % 0x20;
    
    if (PIDGroup >= 7){
        return false;
    }

    return (config.supportedPIDs[PIDGroup] >> (31 - PIDIndex) & 1) == 1;
}

CANReturnStatus sendS1Command(OBD2Config &config, const OBD2_S1Command command){
    if (command == NONE || !isCommandSupported(config,command)){
        return CANReturnStatus::NOK_BAD_CMD;
    }
    
    CAN.beginPacket(config.TxAddress, 8);
    CAN.write(0x02);    //additional bytes in Tx packet
    CAN.write(0x01);    //current data
    CAN.write(command);
    CAN.endPacket();

    return CANReturnStatus::OK;
}

CANReturnStatus receiveS1Command(OBD2Config &config, char *outputBuffer, unsigned long timeout, OBD2_S1Command &commandRx){
    if (!outputBuffer)return CANReturnStatus::NOK_NULL_BUFFER;
    if (config.RxAddress != OBD2_RX_ECM_1) return CANReturnStatus::NOK_NOT_S1_CFG;

    bool packetParsed = CAN.parsePacket() != 0;
    uint8_t RxLength = CAN.read() - 2;
    bool isS1Response = CAN.read() == 0x41;
    commandRx = static_cast<OBD2_S1Command>(CAN.read());
    if (!packetParsed || RxLength < 1 || !isS1Response) return CANReturnStatus::NOK_TIMEOUT;
    for (int i = 0; i < RxLength; i++){
        outputBuffer[i] = CAN.read();
    }
    return CANReturnStatus::OK;
}