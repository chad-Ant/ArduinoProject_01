#include "../include/OBD2Functions.h"
#include "../include/TimerFunctions.h"
#include "../include/MathFunctions.h"

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

CANReturnStatus initializeOBD2(OBD2Config &config, CAN_TxAddress TxAddr, CAN_RxAddress RxAddr, int csPin, int irqPin){
    CAN.setPins(csPin,irqPin);
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
    int PIDGroup = command >> 5;
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

CANReturnStatus receiveS1Command(OBD2Config &config, char *outputBuffer, byte bufferSize, OBD2_S1Command &commandRx, unsigned long timeout){
    if (!outputBuffer)return CANReturnStatus::NOK_NULL_BUFFER;
    if (config.RxAddress != OBD2_RX_ECM_1) return CANReturnStatus::NOK_NOT_S1_CFG;

    bool packetParsed = CAN.parsePacket() != 0;
    uint8_t RxLength = CAN.read() - 2;
    bool isS1Response = CAN.read() == 0x41;
    commandRx = static_cast<OBD2_S1Command>(CAN.read());
    unsigned long timeStart = millis();
    while (!packetParsed || RxLength != bufferSize || !isS1Response){
        if (isTimeout(timeout,timeStart)) return CANReturnStatus::NOK_TIMEOUT;
    }
    
    for (int i = 0; i < bufferSize; i++){
        outputBuffer[i] = CAN.read();
    }
    return CANReturnStatus::OK;
}

CANReturnStatus fetchRPM(OBD2Config &config, float &rpm){
    char buffer[RPM_T];
    OBD2_S1Command receivedCommand = NONE;
    sendS1Command(config,RPM);
    receiveS1Command(config, buffer, RPM_T, receivedCommand);
    if (receivedCommand != RPM) return CANReturnStatus::NOK_BAD_CMD;
    rpm = ((float)buffer[0] * 256.0 + (float)buffer[1]) * 0.25;
    return CANReturnStatus::OK;
}

CANReturnStatus fetchSpeed(OBD2Config &config, float &speed){
    char buffer[SPEED_T];
    OBD2_S1Command receivedCommand = NONE;
    sendS1Command(config,SPEED);
    receiveS1Command(config, buffer, SPEED_T, receivedCommand);
    if (receivedCommand != SPEED) return CANReturnStatus::NOK_BAD_CMD;
    speed = (float)buffer[0];
    return CANReturnStatus::OK;
}

CANReturnStatus fetchGearRatio(OBD2Config &config, float &gearRatio){
    char buffer[GEAR_RTIO_T];
    OBD2_S1Command receivedCommand = NONE;
    sendS1Command(config,GEAR_RTIO);
    receiveS1Command(config, buffer, GEAR_RTIO_T, receivedCommand);
    if (receivedCommand != GEAR_RTIO) return CANReturnStatus::NOK_BAD_CMD;
    gearRatio = ((float)buffer[2] * 256.0 + (float)buffer[3]) * 0.001;
    return CANReturnStatus::OK;
}

CANReturnStatus fetchAirPressure(OBD2Config &config, float &airPressure){
    char buffer[AIR_PRES_T];
    OBD2_S1Command receivedCommand = NONE;
    sendS1Command(config,AIR_PRES);
    receiveS1Command(config, buffer, AIR_PRES_T, receivedCommand);
    if (receivedCommand != AIR_PRES) return CANReturnStatus::NOK_BAD_CMD;
    airPressure = (float)buffer[0];
    return CANReturnStatus::OK;
}

CANReturnStatus fetchODO(OBD2Config &config, float &odo){
    char buffer[ODOMETER_T];
    OBD2_S1Command receivedCommand = NONE;
    sendS1Command(config,ODOMETER);
    receiveS1Command(config, buffer, ODOMETER_T, receivedCommand);
    if (receivedCommand != ODOMETER) return CANReturnStatus::NOK_BAD_CMD;
    odo = (float) ((unsigned int)buffer[0] << 24 + (unsigned int)buffer[1] << 16 + (unsigned int)buffer[2] << 8 + (unsigned int)buffer[3]) * 0.1;
    return CANReturnStatus::OK;
}

CANReturnStatus fetchFuelLvl(OBD2Config &config, float &fuel){
    char buffer[FUEL_LVL_T];
    OBD2_S1Command receivedCommand = NONE;
    sendS1Command(config,FUEL_LVL);
    receiveS1Command(config, buffer, FUEL_LVL_T, receivedCommand);
    if (receivedCommand != FUEL_LVL) return CANReturnStatus::NOK_BAD_CMD;
    fuel = interpolate((float)buffer[0], 255.0, 0.0);
    return CANReturnStatus::OK;
}

CANReturnStatus fetchEngineTemp(OBD2Config &config, float &engineTemp){
    char buffer[ENGINE_TEMP_T];
    OBD2_S1Command receivedCommand = NONE;
    sendS1Command(config,ENGINE_TEMP);
    receiveS1Command(config, buffer, ENGINE_TEMP_T, receivedCommand);
    if (receivedCommand != ENGINE_TEMP) return CANReturnStatus::NOK_BAD_CMD;
    engineTemp = (float)buffer[0] - 40.0;
    return CANReturnStatus::OK;
}

CANReturnStatus fetchFuelRate(OBD2Config &config, float &fuelRate){
    char buffer[FUEL_RATE_T];
    OBD2_S1Command receivedCommand = NONE;
    sendS1Command(config,FUEL_RATE);
    receiveS1Command(config, buffer, FUEL_RATE_T, receivedCommand);
    if (receivedCommand != FUEL_RATE) return CANReturnStatus::NOK_BAD_CMD;
    fuelRate = ((float)buffer[0] * 256 + (float)buffer[1]) * 0.05;
    return CANReturnStatus::OK;
}

CANReturnStatus fetchEngineLoad(OBD2Config &config, float &engineLoad){
    char buffer[ENGINE_LOAD_T];
    OBD2_S1Command receivedCommand = NONE;
    sendS1Command(config,ENGINE_LOAD);
    receiveS1Command(config, buffer, ENGINE_LOAD_T, receivedCommand);
    if (receivedCommand != ENGINE_LOAD) return CANReturnStatus::NOK_BAD_CMD;
    engineLoad = interpolate((float)buffer[0], 255.0, 0.0);
    return CANReturnStatus::OK;
}

CANReturnStatus fetchThrottle(OBD2Config &config, float &throttle){
    char buffer[THROTTLE_POSN_T];
    OBD2_S1Command receivedCommand = NONE;
    sendS1Command(config,THROTTLE_POSN);
    receiveS1Command(config, buffer, THROTTLE_POSN_T, receivedCommand);
    if (receivedCommand != THROTTLE_POSN) return CANReturnStatus::NOK_BAD_CMD;
    throttle = interpolate((float)buffer[0], 255.0, 0.0);
    return CANReturnStatus::OK;
}