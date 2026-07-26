#include "OBD2FunctionsUnderTest.h"

// How often to print the full data table while polling
#define PRINT_INTERVAL_MS  100UL

// ---- globals ----
static OBD2Config    canConfig;
static OBD2Data      obd2Data;
static uint32_t      updateCount = 0;
static unsigned long lastPrint   = 0;

// ---- print helpers ----

static void printSep()  { Serial.println(F("------------------------------------------------")); }
static void pass(const char *s) { Serial.print(F("  PASS: ")); Serial.println(s); }
static void fail(const char *s) { Serial.print(F("  FAIL: ")); Serial.println(s); }
static void info(const char *s) { Serial.print(F("  INFO: ")); Serial.println(s); }

static void printField(const char *label, float val, const char *unit)
{
    Serial.print(F("  "));
    Serial.print(label);
    Serial.print(F(": "));
    if (isnan(val)) {
        Serial.println(F("---"));
    } else {
        Serial.print(val, 2);
        Serial.print(' ');
        Serial.println(unit);
    }
}

static void printAllData()
{
    printSep();
    Serial.print(F("Updates received: ")); Serial.println(updateCount);
    printField("RPM         ", obd2Data.rpm,         "rpm"    );
    printField("Speed       ", obd2Data.speed,       "km/h"   );
    printField("Coolant     ", obd2Data.coolantTemp, "C"      );
    printField("Fuel Level  ", obd2Data.fuelLevel,   "%"      );
    printField("Fuel Rate   ", obd2Data.fuelRate,    "L/h"    );
    printField("Throttle    ", obd2Data.throttle,    "%"      );
    printField("Engine Load ", obd2Data.engineLoad,  "%"      );
    printField("Air Pres    ", obd2Data.airPressure, "kPa"    );
    printField("Gear (cmd)  ", obd2Data.gear,        "(0=N/P)");
    printField("Gear Ratio  ", obd2Data.gearRatio,   ""       );
    printField("Odometer    ", obd2Data.odo,         "km"     );
    printSep();
}

// ---- test 3: isCommandSupported ----

static void testSupportedPIDs()
{
    static const OBD2_S1Command kPIDs[] = {
        RPM, SPEED, ENGINE_TEMP, FUEL_LVL, FUEL_RATE,
        THROTTLE_POSN, ENGINE_LOAD, AIR_PRES, GEAR_CMD, GEAR_RTIO, ODOMETER
    };
    static const char * const kNames[] = {
        "RPM(0x0C)", "SPEED(0x0D)", "ENGINE_TEMP(0x05)", "FUEL_LVL(0x2F)", "FUEL_RATE(0x5E)",
        "THROTTLE(0x11)", "ENG_LOAD(0x04)", "AIR_PRES(0x33)", "GEAR_CMD(0xA3)", "GEAR_RTIO(0xA4)", "ODO(0xA6)"
    };
    static const uint8_t kCount = sizeof(kPIDs) / sizeof(kPIDs[0]);

    for (uint8_t i = 0; i < kCount; i++) {
        Serial.print(F("  "));
        Serial.print(kNames[i]);
        Serial.print(F(" -> "));
        Serial.println(isCommandSupported(canConfig, kPIDs[i]) ? F("supported") : F("not reported"));
    }
}

// ---- setup ----

void setup()
{
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.println(F("=============================================="));
    Serial.println(F("          OBD2 Library Test"));
    Serial.println(F("=============================================="));

    // ---- Test 1: initializeOBD2 ----
    Serial.println(F("\n[1] initializeOBD2"));
    CANReturnStatus s = initializeOBD2(canConfig,
                                       OBD2_TX_GLOBAL,
                                       OBD2_RX_ECM_1,
                                       MCP2515_DEFAULT_CS_PIN,
                                       MCP2515_DEFAULT_INT_PIN);
    if (s == CANReturnStatus::OK) {
        pass("CAN module initialised at 500 kbps");
    } else {
        fail("initializeOBD2 failed — check MCP2515 CS/INT/SPI wiring");
        Serial.print(F("  Status code: ")); Serial.println(static_cast<int>(s));
        // Halt and blink rapidly to signal hardware error.
        while (true) {
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
            delay(150);
        }
    }

    // ---- Test 2: getSupportedPIDs (optional, currently under validation) ----
    Serial.println(F("\n[2] getSupportedPIDs (optional — 2 s per group)"));
    CANReturnStatus ps = getSupportedPIDs(canConfig, 2000UL);
    if (ps == CANReturnStatus::OK) {
        pass("ECU returned PID support bitmasks:");
        for (uint8_t g = 0; g < 7; g++) {
            Serial.print(F("    Group ")); Serial.print(g);
            Serial.print(F(" (PID 0x"));
            Serial.print(g * 0x20, HEX);
            Serial.print(F("): 0x"));
            Serial.println(canConfig.supportedPIDs[g], HEX);
        }
    } else {
        Serial.print(F("  SKIP (code "));
        Serial.print(static_cast<int>(ps));
        Serial.println(F(") — isCommandSupported will show 'not reported'"));
    }

    // ---- Test 3: isCommandSupported ----
    Serial.println(F("\n[3] isCommandSupported for polled PIDs"));
    testSupportedPIDs();

    // ---- Test 4: initOBD2Data ----
    Serial.println(F("\n[4] initOBD2Data"));
    initOBD2Data(obd2Data);
    bool allNaN = isnan(obd2Data.rpm)      && isnan(obd2Data.speed)    &&
                  isnan(obd2Data.coolantTemp) && isnan(obd2Data.fuelLevel) &&
                  isnan(obd2Data.fuelRate)  && isnan(obd2Data.throttle)  &&
                  isnan(obd2Data.engineLoad) && isnan(obd2Data.airPressure) &&
                  isnan(obd2Data.gear)      && isnan(obd2Data.gearRatio) &&
                  isnan(obd2Data.odo);
    allNaN ? pass("all 11 fields initialised to NaN") : fail("some fields not NaN after initOBD2Data");

    // ---- Test 5: resetOBD2Poll then live polling ----
    Serial.println(F("\n[5] resetOBD2Poll + tickOBD2 live poll (500 ms table refresh)"));
    info("--- = not yet received from ECU");
    resetOBD2Poll();
    lastPrint = millis();
}

// ---- loop ----

void loop()
{
    bool updated = tickOBD2(canConfig, obd2Data);
    if (updated) {
        ++updateCount;
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }

    if (isTimeout(PRINT_INTERVAL_MS, lastPrint)) {
        printAllData();
        resetTask(lastPrint);
    }
}
