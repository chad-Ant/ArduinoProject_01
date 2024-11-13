#include "include/HelperFunctions.h"
#include <CAN.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

SFE_UBLOX_GNSS myGNSS;
Adafruit_SSD1306 display(OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGT, &Wire, OLED_RESET);

long lastTime = 0;
bool LEDStatus = false;
char16_t displayString[128];

void setup()
{
  //Serial1 setup
  Serial.begin(SERIAL_BAUDRATE);
  while (!Serial); //Wait for user to open terminal

  pinMode(STATUS_INDICATOR, OUTPUT);

  //OLED screen setup
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while(1);
  }
  Serial.println("OLED initialized");
  display.display();
  delay(1000);
  display.clearDisplay();

  //GPS Shield setup
  pinMode(GPS_PIN, OUTPUT);
  digitalWrite(GPS_PIN, HIGH);

  //Available baudrates: 4800, 9600, 38400, 19200, 57600, 115200, 230400, 460800
  do {
    Serial.println("GNSS: trying 38400 baud");
    Serial1.begin(38400);
    if (myGNSS.begin(Serial1) == true) break;

    delay(5000);

    Serial.println("GNSS: trying 9600 baud");
    Serial1.begin(DEFAULT_GPS_BAUDRATE);
    if (myGNSS.begin(Serial1) == true) {
        Serial.println("GNSS: connected at 9600 baud, switching to custom baud rate");
        myGNSS.setSerialRate(CUSTOM_GPS_BAUDRATE);
        delay(5000);
    } else {
        //myGNSS.factoryReset();
        delay(5000); //Wait a bit before trying again to limit the Serial output
    }

  } while(1);
  Serial.println("GNSS serial connected");

  myGNSS.setUART1Output(COM_TYPE_UBX); //Set the UART port to output UBX only
  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  myGNSS.saveConfiguration(); //Save the current settings to flash and BBR

  //CAN interrupt setup
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
  // register the receive callback
  //CAN.onReceive(onReceive);
}

void loop()
{
  digitalWrite(LED_BUILTIN, LEDStatus);
  //Query module only every second. Doing it more often will just cause I2C traffic.
  //The module only responds when a new position is available
  if (millis() - lastTime > 1000)
  {
    lastTime = millis(); //Update the timer
    
    long latitude = myGNSS.getLatitude();
    Serial.print(F("Lat: "));
    Serial.print(latitude);

    long longitude = myGNSS.getLongitude();
    Serial.print(F(" Long: "));
    Serial.print(longitude);
    Serial.print(F(" (degrees * 10^-7)"));

    long altitude = myGNSS.getAltitudeMSL();
    Serial.print(F(" Alt: "));
    Serial.print(altitude);
    Serial.print(F(" (mm)"));

    long speed = myGNSS.getGroundSpeed();
    long heading = myGNSS.getHeading();

    byte SIV = myGNSS.getSIV();
    Serial.print(F(" SIV: "));
    Serial.print(SIV);
    Serial.println();

    int32ToChar16String(longitude, displayString,12);
    OLED_drawText(displayString);
    LEDStatus = !LEDStatus;
  }
}

void OLED_drawText(char16_t displayString[128]){
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.cp437(true);

    for(int16_t i = 0; i < 12; i++){
        display.write(displayString[i]);
    }

    display.display();
}

void onReceive(int packetSize) {
  // received a packet
  Serial.print("Received ");

  if (CAN.packetExtended()) {
    Serial.print("extended ");
  }

  if (CAN.packetRtr()) {
    // Remote transmission request, packet contains no data
    Serial.print("RTR ");
  }

  Serial.print("packet with id 0x");
  Serial.print(CAN.packetId(), HEX);

  if (CAN.packetRtr()) {
    Serial.print(" and requested length ");
    Serial.println(CAN.packetDlc());
  } else {
    Serial.print(" and length ");
    Serial.println(packetSize);

    // only print packet data for non-RTR packets
    while (CAN.available()) {
      Serial.print((char)CAN.read());
    }
    Serial.println();
  }

  Serial.println();
}