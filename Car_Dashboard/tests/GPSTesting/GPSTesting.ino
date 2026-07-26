#include "GPSFunctionsUnderTest.h"

bool i2cInitialized = false;
bool spiInitialized = false;
bool RTCset = false;

static constexpr uint32_t SERIAL_WAIT_MS = 2000UL;
static constexpr uint32_t INIT_RETRY_MS = 5000UL;
static constexpr uint32_t REPORT_INTERVAL_MS = 1000UL;
static constexpr uint32_t EXPECTED_FIX_WINDOW_MS = 30UL * 60UL * 1000UL;

static SFE_UBLOX_GNSS gnss;
static GPSData gpsData;

static bool gpsReady = false;
static bool receivedPvt = false;
static bool thirtyMinuteNoticePrinted = false;
static uint32_t acquisitionStartedMs = 0;
static uint32_t lastInitAttemptMs = 0;
static uint32_t lastReportMs = 0;
static GPSReturnStatus lastPvtStatus = GPSReturnStatus::DATA_STALE;

static void printTwoDigits(uint8_t value)
{
    if (value < 10)
    {
        Serial.print('0');
    }
    Serial.print(value);
}

static void printElapsed(uint32_t elapsedMs)
{
    const uint32_t elapsedSeconds = elapsedMs / 1000UL;
    const uint32_t minutes = elapsedSeconds / 60UL;
    const uint8_t seconds = static_cast<uint8_t>(elapsedSeconds % 60UL);

    Serial.print(minutes);
    Serial.print(':');
    printTwoDigits(seconds);
}

static void printUtc(const GPSDateTime &utc)
{
    if (!utc.valid)
    {
        Serial.print(F("not valid"));
        return;
    }

    Serial.print(utc.year);
    Serial.print('-');
    printTwoDigits(utc.month);
    Serial.print('-');
    printTwoDigits(utc.day);
    Serial.print('T');
    printTwoDigits(utc.hour);
    Serial.print(':');
    printTwoDigits(utc.minute);
    Serial.print(':');
    printTwoDigits(utc.second);
    Serial.print(F("Z"));
}

static void printInitFailure(GPSReturnStatus status)
{
    Serial.print(F("GPS initialization failed (status "));
    Serial.print(static_cast<int>(status));
    Serial.println(F("). Retrying in 5 seconds."));

    if (status == GPSReturnStatus::NOK_INIT_FAILED)
    {
        Serial.println(F("Check the Eslov cable, shield power, and I2C address 0x42."));
    }
    else if (status == GPSReturnStatus::NOK_CONFIG_FAILED)
    {
        Serial.println(F("The receiver answered on I2C but rejected its configuration."));
    }
}

static bool attemptGpsInit()
{
    lastInitAttemptMs = millis();
    const GPSReturnStatus status = initializeGPS_I2C(gnss);

    if (status != GPSReturnStatus::OK)
    {
        printInitFailure(status);
        return false;
    }

    initGPSData(gpsData);
    gpsReady = true;
    receivedPvt = false;
    thirtyMinuteNoticePrinted = false;
    acquisitionStartedMs = millis();
    lastReportMs = acquisitionStartedMs - REPORT_INTERVAL_MS;

    Serial.println(F("GPS initialized over the Eslov I2C connection."));
    Serial.println(F("Waiting indefinitely for a valid fix; up to 30 minutes is expected."));
    Serial.println(F("Place the antenna outdoors with a clear view of the sky."));
    Serial.println(F("Heading is course over ground and is meaningful only while moving."));
    return true;
}

static void printSnapshot(uint32_t now)
{
    Serial.print(F("elapsed="));
    printElapsed(now - acquisitionStartedMs);
    Serial.print(F("  satellites="));
    Serial.print(gpsData.satellites);
    Serial.print(F("  fixType="));
    Serial.print(gpsData.fixType);
    Serial.print(F("  UTC="));
    printUtc(gpsData.utc);

    Serial.print(F("  signal="));
    Serial.print(static_cast<int>(evaluateSignal(gpsData)));
    if (!receivedPvt)
    {
        Serial.println(F("  waiting for first PVT packet"));
        return;
    }

    if (lastPvtStatus == GPSReturnStatus::NO_FIX)
    {
        Serial.println(F("  waiting for valid fix"));
        return;
    }

    if (lastPvtStatus != GPSReturnStatus::OK)
    {
        Serial.print(F("  PVT status="));
        Serial.println(static_cast<int>(lastPvtStatus));
        return;
    }

    Serial.println();
    Serial.print(F("  latitude="));
    Serial.print(gpsData.latitudeDegrees, 7);
    Serial.print(F(" deg  longitude="));
    Serial.print(gpsData.longitudeDegrees, 7);
    Serial.print(F(" deg  altitude="));
    Serial.print(gpsData.altitudeM, 2);
    Serial.println(F(" m"));
    Serial.print(F("  velocity="));
    Serial.print(gpsData.velocityKmh, 2);
    Serial.print(F(" km/h  heading="));
    Serial.print(gpsData.headingDegrees, 2);
    Serial.println(F(" deg"));
}

void setup()
{
    Serial.begin(115200);
    const uint32_t serialStartedMs = millis();
    while (!Serial && (millis() - serialStartedMs < SERIAL_WAIT_MS))
    {
    }

    Serial.println();
    Serial.println(F("MKR GPS Shield I2C/Eslov hardware soak test"));
    initGPSData(gpsData);
    attemptGpsInit();
}

void loop()
{
    const uint32_t now = millis();

    if (!gpsReady)
    {
        if (now - lastInitAttemptMs >= INIT_RETRY_MS)
        {
            attemptGpsInit();
        }
        return;
    }

    const GPSReturnStatus currentStatus = getGPSData(gnss, gpsData);
    if (currentStatus != GPSReturnStatus::DATA_STALE)
    {
        receivedPvt = true;
        lastPvtStatus = currentStatus;
    }

    if (now - lastReportMs >= REPORT_INTERVAL_MS)
    {
        lastReportMs = now;
        printSnapshot(now);
    }

    if (!gpsData.fixValid &&
        !thirtyMinuteNoticePrinted &&
        now - acquisitionStartedMs >= EXPECTED_FIX_WINDOW_MS)
    {
        thirtyMinuteNoticePrinted = true;
        Serial.println(F("No fix after 30 minutes; continuing indefinitely."));
        Serial.println(F("Recheck the outdoor sky view, Eslov cable, and antenna orientation."));
    }
}
