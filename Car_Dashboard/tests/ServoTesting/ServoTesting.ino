#include "include/ServoFunctions.h"
#include "include/TimerFunctions.h"

Servo servo1;
PinStatus LED_on = HIGH;
unsigned long lastRun = millis();
unsigned long lastWrite = millis();
int positionControl = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,LED_on);
  if (initializeServo(servo1,16)){
    Serial.println("Attached!");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (isTimeout(100,lastRun)){
    int position = readPosition(servo1);
    Serial.println(position);
    if (position >= 90){
      digitalWrite(LED_BUILTIN,HIGH);
    } else {
      digitalWrite(LED_BUILTIN,LOW);
    }
    resetTask(lastRun);
  }

  if (isTimeout(1000,lastWrite)){
    positionControl = positionControl > 180 ? 0 : positionControl + 10;
    writePosition(servo1,positionControl);
    resetTask(lastWrite);
  }
}
