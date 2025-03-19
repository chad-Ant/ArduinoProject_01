#include <SPI.h>

const int chipSelectPin = 7;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(chipSelectPin, OUTPUT);
  digitalWrite(chipSelectPin, HIGH);
  SPI.begin();
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(chipSelectPin, LOW);
  uint16_t rawData = SPI.transfer16(0x0000);
  digitalWrite(chipSelectPin, HIGH);
  uint8_t data = (rawData >> 5) & 0xFF;
  Serial.println(rawData, HEX);
  Serial.println(data);
  delay(100);
}
