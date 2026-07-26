#include "SegmentLEDFunctionsUnderTest.h"

// Deterministic tests for the PRODUCTION SegmentLED library. Each render writes into
// the Adafruit display buffer (alpha4.displaybuffer[0..3]) before the I2C push, so the
// rendering logic is asserted in RAM — no physical display required. If a display IS
// present at 0x70 the renders also appear on it.

static Adafruit_AlphaNum4 alpha4;
static int g_pass = 0;
static int g_fail = 0;

static void report(const char *name, bool ok)
{
    Serial.print(ok ? F("[PASS] ") : F("[FAIL] "));
    Serial.println(name);
    if (ok) ++g_pass; else ++g_fail;
}

static void snap(uint16_t out[4])
{
    for (int i = 0; i < 4; ++i) out[i] = alpha4.displaybuffer[i];
}

// Exact check of the four rendered digits (used for writeDigitRaw-based renders,
// where the buffer equals the chosen DigitMapping value).
static void expectBuf(const char *name, uint16_t b0, uint16_t b1, uint16_t b2, uint16_t b3)
{
    uint16_t d[4];
    snap(d);
    bool ok = (d[0] == b0 && d[1] == b1 && d[2] == b2 && d[3] == b3);
    report(name, ok);
    if (!ok) {
        Serial.print(F("       got  0x")); Serial.print(d[0], HEX);
        Serial.print(F(" 0x")); Serial.print(d[1], HEX);
        Serial.print(F(" 0x")); Serial.print(d[2], HEX);
        Serial.print(F(" 0x")); Serial.println(d[3], HEX);
        Serial.print(F("       want 0x")); Serial.print(b0, HEX);
        Serial.print(F(" 0x")); Serial.print(b1, HEX);
        Serial.print(F(" 0x")); Serial.print(b2, HEX);
        Serial.print(F(" 0x")); Serial.println(b3, HEX);
    }
}

void setup()
{
    Serial.begin(115200);
    const uint32_t started = millis();
    while (!Serial && (millis() - started < 2000)) {}

    Serial.println();
    Serial.println(F("SegmentLED production-source buffer tests"));
    bool found = alpha4.begin(SEGLED_ADDRESS);
    Serial.println(found ? F("Display found at 0x70 (renders also shown on it).")
                         : F("No display at 0x70 - buffer logic still tested in RAM."));
    Serial.println();

    // ---- writeFloatLED_Mirror: writeDigitRaw path, exact bitmaps ----
    writeFloatLED_Mirror(alpha4, 0.0f);
    expectBuf("mirror float 0",      NUM_0_DP,           NUM_0,          NONE_TO_DISPLAY, NONE_TO_DISPLAY);
    writeFloatLED_Mirror(alpha4, 12.5f);
    expectBuf("mirror float 12.5",   NUM_5_DP_MIRROR,    NUM_2_MIRROR,   NUM_1_MIRROR,    NONE_TO_DISPLAY);
    writeFloatLED_Mirror(alpha4, 1234.0f);
    expectBuf("mirror float 1234",   NUM_4_MIRROR,       NUM_3_MIRROR,   NUM_2_MIRROR,    NUM_1_MIRROR);
    writeFloatLED_Mirror(alpha4, NAN);
    expectBuf("mirror NaN",          CHAR_N_MIRROR,      CHAR_A_MIRROR,  CHAR_N_MIRROR,   NONE_TO_DISPLAY);
    writeFloatLED_Mirror(alpha4, INFINITY);
    expectBuf("mirror +INF",         CHAR_F_MIRROR,      CHAR_N_MIRROR,  CHAR_I_MIRROR,   PLUS_CROSS);
    writeFloatLED_Mirror(alpha4, -INFINITY);
    expectBuf("mirror -INF",         CHAR_F_MIRROR,      CHAR_N_MIRROR,  CHAR_I_MIRROR,   MINUS_SIGN);

    // ---- writeStringLED_Mirror: the short-string bounds-bug target ----
    writeStringLED_Mirror(alpha4, "");
    expectBuf("mirror str \"\"",     NONE_TO_DISPLAY,    NONE_TO_DISPLAY, NONE_TO_DISPLAY, NONE_TO_DISPLAY);
    writeStringLED_Mirror(alpha4, "A");
    expectBuf("mirror str \"A\"",    NONE_TO_DISPLAY,    NONE_TO_DISPLAY, NONE_TO_DISPLAY, CHAR_A_MIRROR);
    writeStringLED_Mirror(alpha4, "AB");
    expectBuf("mirror str \"AB\"",   NONE_TO_DISPLAY,    NONE_TO_DISPLAY, CHAR_B_MIRROR,   CHAR_A_MIRROR);
    writeStringLED_Mirror(alpha4, "1234");
    expectBuf("mirror str \"1234\"", NUM_4_MIRROR,       NUM_3_MIRROR,   NUM_2_MIRROR,    NUM_1_MIRROR);

    // ---- writeFloatLED: non-mirror number path, exact bitmaps ----
    writeFloatLED(alpha4, 123.5f);
    expectBuf("float 123.5",         NUM_1,              NUM_2,          NUM_3_DP,        NUM_5);

    // ---- writeFloatLED special values: ASCII-font path, relational ----
    {
        uint16_t pinf[4], ninf[4], nanb[4], zero[4];
        writeFloatLED(alpha4, INFINITY);  snap(pinf);
        writeFloatLED(alpha4, -INFINITY); snap(ninf);
        report("float +INF/-INF differ at digit 0 (+ vs -)",
               pinf[0] != ninf[0] && pinf[0] != 0 && ninf[0] != 0);
        report("float INF digits I,N,F match",
               pinf[1] == ninf[1] && pinf[2] == ninf[2] && pinf[3] == ninf[3] && pinf[1] != 0);
        writeFloatLED(alpha4, NAN);        snap(nanb);
        report("float NaN ('N','A','N')", nanb[0] == nanb[2] && nanb[0] != 0 && nanb[1] != 0 && nanb[3] == 0);
        writeFloatLED(alpha4, 0.0f);       snap(zero);
        report("float 0 -> '0.' + '0'",
               (zero[0] & 0x4000) != 0 && (zero[0] & 0x3FFF) == zero[1] && zero[1] != 0 && zero[2] == 0 && zero[3] == 0);
    }

    // ---- writeStringLED: non-mirror ASCII path, short strings + 4-char limit ----
    {
        uint16_t d[4];
        writeStringLED(alpha4, "");     snap(d);
        report("str \"\" blank",        d[0] == 0 && d[1] == 0 && d[2] == 0 && d[3] == 0);
        writeStringLED(alpha4, "A");    snap(d);
        report("str \"A\" (1 digit)",   d[0] != 0 && d[1] == 0 && d[2] == 0 && d[3] == 0);
        writeStringLED(alpha4, "AB");   snap(d);
        report("str \"AB\" (2 digits)", d[0] != 0 && d[1] != 0 && d[2] == 0 && d[3] == 0);
        writeStringLED(alpha4, "ABCD"); snap(d);
        report("str \"ABCD\" (4)",      d[0] != 0 && d[1] != 0 && d[2] != 0 && d[3] != 0);
        writeStringLED(alpha4, "ABCDE");snap(d);
        report("str \"ABCDE\" -> first 4 only", d[0] != 0 && d[1] != 0 && d[2] != 0 && d[3] != 0);
    }

    // ---- mirrored and non-mirrored renders must differ for the same value ----
    {
        uint16_t nm[4], mm[4];
        writeFloatLED(alpha4, 12.0f);        snap(nm);
        writeFloatLED_Mirror(alpha4, 12.0f); snap(mm);
        report("mirror vs non-mirror differ (12.0)",
               !(nm[0] == mm[0] && nm[1] == mm[1] && nm[2] == mm[2] && nm[3] == mm[3]));
    }

    Serial.println();
    Serial.print(F("RESULT: "));
    Serial.print(g_pass);
    Serial.print(F(" passed, "));
    Serial.print(g_fail);
    Serial.println(g_fail == 0 ? F(" failed  => ALL TESTS PASSED") : F(" failed  => FAILURES PRESENT"));
}

void loop()
{
    static uint32_t last = 0;
    if (millis() - last >= 1000UL) {
        last = millis();
        Serial.print(F("[done] "));
        Serial.print(g_pass);
        Serial.print('/');
        Serial.print(g_pass + g_fail);
        Serial.println(F(" passed"));
    }
}
