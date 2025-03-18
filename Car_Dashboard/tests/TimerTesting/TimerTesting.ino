#include "TimerFunctions.h"

unsigned long timeLED = millis();
unsigned long timeSerial = millis();
int LEDState = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (hasTimeElapsed(1000,timeLED)){
    LEDState ^= 1;
    digitalWrite(LED_BUILTIN, LEDState);  // turn the LED on (HIGH is the voltage level)
    timeLED = millis();
  }
  
  if (hasTimeElapsed(2102,timeSerial)){
    Serial.println(timeSerial);
    timeSerial = millis();
  }
}
