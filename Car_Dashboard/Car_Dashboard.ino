#include "config/DataDictionary.h"
//#include "include/WiFiFunctions.h"
//#include "include/HTTPClientFunctions.h"
#include "include/TimerFunctions.h" //works
#include "include/GPSFunctions.h"
#include "include/MathFunctions.h"
//#include "include/ServoFunctions.h"
#include "include/OBD2Functions.h"
//#include "include/SignalProcessingFunctions.h"
#include "include/CommunicationFunctions.h"

//RTCZero rtc;
SFE_UBLOX_GNSS myGNSS;
OBD2Config OBD2S1Commands;
OBD2Data   obd2Data;
GPSData    gpsData;
CommMaster commMaster;         // ESP32-C3 telemetry link (Serial1)
//IPAddress localIP;
//SimpleMovingAverage SMA1(SIZE_16);
//PIDControls heightControl(25.0, 0.5, 0.04, 5.0, 20.0, 0.1);

unsigned long lastLEDBlink = 0, lastStatusPrint = 0;
int LED_on = 1;

bool obdReady = false;                 // true only after initializeOBD2() succeeds
unsigned long lastObdInitMs = 0;
const unsigned long OBD_REINIT_INTERVAL_MS = 5000UL;

// --- SAMD21 hardware watchdog: resets the board if loop() stalls for ~4 s — a
//     last-resort guard for any wait the software bounds cannot cover. ---
static void watchdogSetup()
{
  // Clock the WDT from the always-on ultra-low-power 32.768 kHz oscillator via
  // GCLK2, divided by 32 → 1024 Hz.
  GCLK->GENDIV.reg  = GCLK_GENDIV_ID(2) | GCLK_GENDIV_DIV(4);          // 2^(4+1) = 32
  while (GCLK->STATUS.bit.SYNCBUSY) {}
  GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(2) | GCLK_GENCTRL_GENEN |
                      GCLK_GENCTRL_SRC_OSCULP32K | GCLK_GENCTRL_DIVSEL;
  while (GCLK->STATUS.bit.SYNCBUSY) {}
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(WDT_GCLK_ID) | GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK2;
  while (GCLK->STATUS.bit.SYNCBUSY) {}

  WDT->CTRL.reg = 0;                       // disable while configuring
  while (WDT->STATUS.bit.SYNCBUSY) {}
  WDT->INTENCLR.bit.EW = 1;                // no early-warning interrupt
  WDT->CONFIG.reg = WDT_CONFIG_PER_4K;     // 4096 / 1024 Hz ≈ 4 s
  WDT->CTRL.reg = WDT_CTRL_ENABLE;         // enable, normal (non-windowed) mode
  while (WDT->STATUS.bit.SYNCBUSY) {}
}

static inline void watchdogReset()
{
  if (!WDT->STATUS.bit.SYNCBUSY) WDT->CLEAR.reg = WDT_CLEAR_CLEAR_KEY;
}

void setup()
{
  pinMode(STATUS_INDICATOR, OUTPUT);
  digitalWrite(STATUS_INDICATOR,1);

  //Debug setup — bounded wait so the node still boots when running headless (no USB host).
  Serial.begin(SERIAL_BAUDRATE);
  const unsigned long serialStart = millis();
  while (!Serial && (millis() - serialStart < 3000UL)) {}
  Serial.println("Serial started.");
  //Wifi setup
  //if (initializeWifi() == WiFiReturnStatus::OK) Serial.println("Wifi connected.");
  //else Serial.println("Wifi not connected.");

  //RTC setup
  //initializeRTC(rtc);
  //if (setRTCDateTime(rtc) == TimerReturnStatus::OK) Serial.println("RTC set!");
  //else Serial.println("RTC not set up.");

  //GPS setup (I2C / Eslov on the MKR Zero)
  if (initializeGPS_I2C(myGNSS) == GPSReturnStatus::OK) Serial.println("GPS module started.");
  else Serial.println("GPS module failed.");
  initGPSData(gpsData);

  //CAN / OBD2 setup
  obdReady = (initializeOBD2(OBD2S1Commands, OBD2_TX_GLOBAL, OBD2_RX_ECM_1) == CANReturnStatus::OK);
  Serial.println(obdReady ? "CAN module starts" : "CAN module init failed.");
  lastObdInitMs = millis();
  initOBD2Data(obd2Data);

  //ESP32-C3 telemetry link
  initializeComm();
  initCommMaster(commMaster);

  watchdogSetup();   // arm the ~4 s hardware watchdog last, once all init is done
}

void loop()
{
  watchdogReset();   // pat the watchdog every iteration; a >4 s stall forces a reset

  // Refresh the data sources (both non-blocking) …
  // OBD2: poll while the controller is up. A run-time bus loss (many consecutive TX
  // failures) drops us back into the bounded 5 s re-initialisation path.
  if (obdReady) {
    tickOBD2(OBD2S1Commands, obd2Data);
    if (isOBD2LinkLost()) {
      obdReady = false;
      lastObdInitMs = millis();
      initOBD2Data(obd2Data);   // clear stale readings (also clears the freshness flag)
    }
  } else if (millis() - lastObdInitMs >= OBD_REINIT_INTERVAL_MS) {
    lastObdInitMs = millis();
    obdReady = (initializeOBD2(OBD2S1Commands, OBD2_TX_GLOBAL, OBD2_RX_ECM_1) == CANReturnStatus::OK);
    if (obdReady) { resetOBD2Poll(); initOBD2Data(obd2Data); }
  }
  getGPSData(myGNSS, gpsData);

  // … then service the ESP32-C3: replies to its commands and, once it has
  // requested a stream, pushes combined OBD2+GPS telemetry at 10 Hz.
  tickCommMaster(commMaster, obd2Data, gpsData);

  if (isTimeout(2000,lastLEDBlink)){
    LED_on ^= 1;
    digitalWrite(STATUS_INDICATOR,LED_on);
    lastLEDBlink = millis();
  }

  if (isTimeout(1000,lastStatusPrint)){
    lastStatusPrint = millis();
    Serial.print("spd="); Serial.print(obd2Data.speed);
    Serial.print(" km/h  sats="); Serial.print(gpsData.satellites);
    Serial.print(" streaming="); Serial.println(commMaster.streaming);
  }
}
