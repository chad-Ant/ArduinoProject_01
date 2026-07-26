#include "CommTestSupport.h"

// On-device unit test for the CommProtocol wire layer (framing, CRC, resync,
// TelemetryPayload round-trip). Runs entirely in RAM on the MKR Zero — no
// wiring required. Prints PASS/FAIL over USB Serial.

static constexpr uint32_t SERIAL_WAIT_MS = 2000UL;

static int g_pass = 0;
static int g_fail = 0;

static void report(const char *name, bool ok)
{
    Serial.print(ok ? F("[PASS] ") : F("[FAIL] "));
    Serial.println(name);
    if (ok) ++g_pass; else ++g_fail;
}

// Feed a byte buffer through a decoder, tallying outcomes (FeedResult lives in
// CommTestSupport.h to satisfy Arduino's prototype ordering).
static void feedAll(CommRxState &rx, const uint8_t *bytes, size_t n, uint8_t cap, FeedResult &res)
{
    res.frames = 0;
    res.sawCrc = false;
    res.sawOverflow = false;
    res.lastType = 0;
    res.lastLen = 0;

    for (size_t i = 0; i < n; ++i) {
        uint8_t t = 0, l = 0;
        uint8_t pl[COMM_MAX_PAYLOAD];
        CommReturnStatus r = commRxByte(rx, bytes[i], t, pl, cap, l);
        if (r == CommReturnStatus::FRAME_READY) {
            ++res.frames;
            res.lastType = t;
            res.lastLen  = l;
            memcpy(res.lastPayload, pl, l);
        } else if (r == CommReturnStatus::NOK_CRC) {
            res.sawCrc = true;
        } else if (r == CommReturnStatus::NOK_OVERFLOW) {
            res.sawOverflow = true;
        }
    }
}

static TelemetryPayload makeSample()
{
    TelemetryPayload p;
    memset(&p, 0, sizeof(p));
    p.masterMillis = 123456UL;
    p.speed = 88.5f;  p.rpm = 2500.0f;  p.coolantTemp = 90.0f;
    p.fuelLevel = 42.0f; p.fuelRate = 5.5f; p.throttle = 12.5f;
    p.engineLoad = 33.0f; p.airPressure = 101.3f; p.gear = 4.0f;
    p.gearRatio = 1.31f; p.odo = 12345.6f;
    p.latitude = 10.7262659f; p.longitude = 107.2564163f; p.altitude = 176.1f;
    p.gpsSpeedKmh = 0.2f; p.heading = 267.47f;
    p.satellites = 11; p.fixType = 3; p.fixValid = 1;
    p.year = 2026; p.month = 7; p.day = 24; p.hour = 15; p.minute = 6; p.second = 0;
    p.flags = COMM_FLAG_OBD2_VALID | COMM_FLAG_GPS_FIX | COMM_FLAG_TIME_VALID;
    return p;
}

static void runTests()
{
    uint8_t frame[COMM_MAX_FRAME];
    CommRxState rx;

    // 1) Telemetry round-trip is bit-exact.
    {
        TelemetryPayload src = makeSample();
        size_t n = buildFrame(MSG_TELEMETRY, reinterpret_cast<const uint8_t*>(&src), sizeof(src), frame, sizeof(frame));
        commRxInit(rx);
        FeedResult res;
        feedAll(rx, frame, n, COMM_MAX_PAYLOAD, res);
        bool ok = (n == COMM_FRAME_OVERHEAD + sizeof(src)) &&
                  res.frames == 1 &&
                  res.lastType == MSG_TELEMETRY &&
                  res.lastLen == sizeof(src) &&
                  memcmp(res.lastPayload, &src, sizeof(src)) == 0;
        report("telemetry round-trip (bit-exact)", ok);
    }

    // 2) A single bit flip in the payload is rejected by CRC.
    {
        TelemetryPayload src = makeSample();
        size_t n = buildFrame(MSG_TELEMETRY, reinterpret_cast<const uint8_t*>(&src), sizeof(src), frame, sizeof(frame));
        frame[6] ^= 0x01; // corrupt a payload byte
        commRxInit(rx);
        FeedResult res;
        feedAll(rx, frame, n, COMM_MAX_PAYLOAD, res);
        report("CRC rejects a bit flip", res.frames == 0 && res.sawCrc);
    }

    // 3) Parser resyncs after leading noise (stray SOF + bad version bytes).
    {
        TelemetryPayload src = makeSample();
        size_t fn = buildFrame(MSG_TELEMETRY, reinterpret_cast<const uint8_t*>(&src), sizeof(src), frame, sizeof(frame));
        const uint8_t noise[] = { 0x00, 0x7E, 0x02, 0x55, 0x7E, 0x99, 0x00, 0xAB };
        uint8_t buf[sizeof(noise) + COMM_MAX_FRAME];
        memcpy(buf, noise, sizeof(noise));
        memcpy(buf + sizeof(noise), frame, fn);
        commRxInit(rx);
        FeedResult res;
        feedAll(rx, buf, sizeof(noise) + fn, COMM_MAX_PAYLOAD, res);
        bool ok = res.frames == 1 &&
                  res.lastType == MSG_TELEMETRY &&
                  memcmp(res.lastPayload, &src, sizeof(src)) == 0;
        report("resync after leading noise", ok);
    }

    // 4) Zero-length command frame round-trips.
    {
        size_t n = buildFrame(CMD_PING, nullptr, 0, frame, sizeof(frame));
        commRxInit(rx);
        FeedResult res;
        feedAll(rx, frame, n, COMM_MAX_PAYLOAD, res);
        report("zero-length command frame", res.frames == 1 && res.lastType == CMD_PING && res.lastLen == 0);
    }

    // 5) Two frames back-to-back both decode from one decoder.
    {
        TelemetryPayload src = makeSample();
        uint8_t two[COMM_MAX_FRAME * 2];
        size_t n1 = buildFrame(MSG_TELEMETRY, reinterpret_cast<const uint8_t*>(&src), sizeof(src), two, sizeof(two));
        size_t n2 = buildFrame(CMD_GET_ONCE, nullptr, 0, two + n1, sizeof(two) - n1);
        commRxInit(rx);
        FeedResult res;
        feedAll(rx, two, n1 + n2, COMM_MAX_PAYLOAD, res);
        report("two frames back-to-back", res.frames == 2 && res.lastType == CMD_GET_ONCE);
    }

    // 6) A payload longer than the caller's capacity is rejected, not overflowed.
    {
        uint8_t payload[20];
        for (uint8_t i = 0; i < sizeof(payload); ++i) payload[i] = i;
        size_t n = buildFrame(0x55, payload, sizeof(payload), frame, sizeof(frame));
        commRxInit(rx);
        FeedResult res;
        feedAll(rx, frame, n, /*cap=*/10, res); // caller buffer smaller than payload
        report("oversized payload rejected (cap)", res.frames == 0 && res.sawOverflow);
    }
}

void setup()
{
    Serial.begin(115200);
    const uint32_t started = millis();
    while (!Serial && (millis() - started < SERIAL_WAIT_MS)) {}

    Serial.println();
    Serial.println(F("CommProtocol wire-layer self-test (MKR Zero)"));
    Serial.print(F("sizeof(TelemetryPayload) = "));
    Serial.println((unsigned)sizeof(TelemetryPayload));
    Serial.println();

    runTests();

    Serial.println();
    Serial.print(F("RESULT: "));
    Serial.print(g_pass);
    Serial.print(F(" passed, "));
    Serial.print(g_fail);
    Serial.println(g_fail == 0 ? F(" failed  => ALL TESTS PASSED") : F(" failed  => FAILURES PRESENT"));
}

void loop()
{
    // Re-print the summary once a second so a late serial connection still sees it.
    static uint32_t last = 0;
    if (millis() - last >= 1000UL) {
        last = millis();
        Serial.print(F("[done] "));
        Serial.print(g_pass);
        Serial.print(F(" passed, "));
        Serial.print(g_fail);
        Serial.println(F(" failed"));
    }
}
