#include "include/SegmentLEDFunctions.h"
#include "include/TimerFunctions.h"
#include "include/QuickMathFunctions.h"

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();
PinStatus LEDStatus = HIGH;
unsigned long lastRun = millis();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if (bool SegLEDStartSuccess = initializeSegmentLED(alpha4)){
    Serial.println("SegLED started.");
    clearSegmentLED(alpha4);
    adjustLEDBrightness(alpha4, 255);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (isTimeout(80,lastRun)){
    //writeFloatLED(alpha4, (float)(counterU8())); //OK
    //writeFloatLED_Mirror(alpha4, (float)(counterU8())); //OK
    //writeStringLED(alpha4, "AqrX"); //OK
    //writeStringLED_Mirror(alpha4, "RDY"); //OK
    Serial.println(1);
    resetTask(lastRun);
  }
}
