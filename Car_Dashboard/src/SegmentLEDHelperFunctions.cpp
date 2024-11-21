#include "..\include\HelperFunctions.h"
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <Arduino.h>
#include <cmath>

enum DigitMapping
{
    NUM_0_MIRROR = 0x213F,
    NUM_0_DP_MIRROR = 0x613F,
    NUM_1_MIRROR = 0x130,
    NUM_1_DP_MIRROR = 0x4130,
    NUM_2_MIRROR = 0x2069,
    NUM_2_DP_MIRROR = 0x6069,
    NUM_3_MIRROR = 0x79,
    NUM_3_DP_MIRROR = 0x4079,
    NUM_4_MIRROR = 0xF2,
    NUM_4_DP_MIRROR = 0x40F2,
    NUM_5_MIRROR = 0x88B,
    NUM_5_DP_MIRROR = 0x488B,
    NUM_6_MIRROR = 0x88F,
    NUM_6_DP_MIRROR = 0x488F,
    NUM_7_MIRROR = 0x2101,
    NUM_7_DP_MIRROR = 0x6101,
    NUM_8_MIRROR = 0xFF,
    NUM_8_DP_MIRROR = 0x40FF,
    NUM_9_MIRROR = 0x479,
    NUM_9_DP_MIRROR = 0x4479,
    CHAR_A_MIRROR = 0xF7,
    CHAR_B_MIRROR = 0x1DF,
    CHAR_C_MIRROR = 0xF,
    CHAR_D_MIRROR = 0xC0E,
    CHAR_E_MIRROR = 0x8F,
    CHAR_F_MIRROR = 0x87,
    CHAR_G_MIRROR = 0x5F,
    CHAR_H_MIRROR = 0xF6,
    CHAR_I_MIRROR = 0x1209,
    CHAR_J_MIRROR = 0x3C,
    CHAR_K_MIRROR = 0x986,
    CHAR_L_MIRROR = 0xE,
    CHAR_M_MIRROR = 0x536,
    CHAR_N_MIRROR = 0xC36,
    CHAR_O_MIRROR = 0x3F,
    CHAR_P_MIRROR = 0xE7,
    CHAR_Q_MIRROR = 0x83F,
    CHAR_R_MIRROR = 0x8E7,
    CHAR_S_MIRROR = 0xDB,
    CHAR_T_MIRROR = 0x1201,
    CHAR_U_MIRROR = 0x3E,
    CHAR_V_MIRROR = 0x2106,
    CHAR_W_MIRROR = 0x2836,
    CHAR_X_MIRROR = 0x2D00,
    CHAR_Y_MIRROR = 0x1500,
    CHAR_Z_MIRROR = 0x2109,
    ALL_SEGMENTS = 0x7FFF,
    X_CROSS = 0x2D00,
    PLUS_CROSS = 0x12C0,
    STAR_5 = 0x2AC0,
    STAR_8 = 0x3FC0,
    EXPONENT_E_MIRROR = 0x208C,
    MINUS_SIGN = 0x80,
    NONE_TO_DISPLAY = 0x0
};

const uint8_t LUT_5_15[256] = {
    5, 6, 7, 7, 8, 8, 9, 9, 9, 9, 9, 9, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15};

DigitMapping mapDigit(int digit, bool decimalPoint)
{
    switch (digit)
    {
    case 0:
        return decimalPoint ? NUM_0_DP_MIRROR : NUM_0_MIRROR;
    case 1:
        return decimalPoint ? NUM_1_DP_MIRROR : NUM_1_MIRROR;
    case 2:
        return decimalPoint ? NUM_2_DP_MIRROR : NUM_2_MIRROR;
    case 3:
        return decimalPoint ? NUM_3_DP_MIRROR : NUM_3_MIRROR;
    case 4:
        return decimalPoint ? NUM_4_DP_MIRROR : NUM_4_MIRROR;
    case 5:
        return decimalPoint ? NUM_5_DP_MIRROR : NUM_5_MIRROR;
    case 6:
        return decimalPoint ? NUM_6_DP_MIRROR : NUM_6_MIRROR;
    case 7:
        return decimalPoint ? NUM_7_DP_MIRROR : NUM_7_MIRROR;
    case 8:
        return decimalPoint ? NUM_8_DP_MIRROR : NUM_8_MIRROR;
    case 9:
        return decimalPoint ? NUM_9_DP_MIRROR : NUM_9_MIRROR;
    default:
        return NONE_TO_DISPLAY;
    }
}

bool initializeSegmentLED(Adafruit_AlphaNum4 &alpha4)
{
    if (!alpha4.begin(SEGLED_ADDRESS))
        return false;
    alpha4.setBrightness(15);
    alpha4.clear();
    alpha4.writeDigitRaw(0, ALL_SEGMENTS);
    alpha4.writeDigitRaw(1, ALL_SEGMENTS);
    alpha4.writeDigitRaw(2, ALL_SEGMENTS);
    alpha4.writeDigitRaw(3, ALL_SEGMENTS);
    alpha4.writeDisplay();

    delay(500);

    alpha4.clear();
    alpha4.writeDisplay();

    return true;
}

void adjustLEDBrightness(Adafruit_AlphaNum4 &alpha4, uint8_t ambientLuminosity)
{
    alpha4.setBrightness(LUT_5_15[ambientLuminosity]);
}

void writeFloatLED_Mirror(Adafruit_AlphaNum4 &alpha4, float number)
{
    static DigitMapping LEDBuffer[4] = {
        NONE_TO_DISPLAY,
        NONE_TO_DISPLAY,
        NONE_TO_DISPLAY,
        NONE_TO_DISPLAY};
    static int integerPart, decimalPart, units, tens, hundreds, thousands;

    if (isinf(number))
    {
        LEDBuffer[0] = CHAR_F_MIRROR;
        LEDBuffer[1] = CHAR_N_MIRROR;
        LEDBuffer[2] = CHAR_I_MIRROR;
        LEDBuffer[3] = PLUS_CROSS;
    }
    else if (isnan(number))
    {
        LEDBuffer[0] = CHAR_N_MIRROR;
        LEDBuffer[1] = CHAR_A_MIRROR;
        LEDBuffer[2] = CHAR_N_MIRROR;
    }
    else if (isinf(-number))
    {
        LEDBuffer[0] = CHAR_F_MIRROR;
        LEDBuffer[1] = CHAR_N_MIRROR;
        LEDBuffer[2] = CHAR_I_MIRROR;
        LEDBuffer[3] = MINUS_SIGN;
    }
    else
    {
        number >= -999 ? (number <= 9999 ? number : 9999) : -999;

        if (number <= -100)
        {
            integerPart = abs((int)number);
            hundreds = div100Approx(integerPart);
            tens = div10Approx(integerPart - hundreds * 100);
            units = integerPart - hundreds * 100 - tens * 10;

            LEDBuffer[3] = MINUS_SIGN;
            LEDBuffer[2] = mapDigit(hundreds, false);
            LEDBuffer[1] = mapDigit(tens, false);
            LEDBuffer[0] = mapDigit(units, false);
        }

        else if (number >= 1000)
        {
            integerPart = (int)number;
            thousands = div1000Approx(integerPart);
            hundreds = div100Approx(integerPart - thousands * 1000);
            tens = div10Approx(integerPart - thousands * 1000 - hundreds * 100);
            units = integerPart - thousands * 1000 - hundreds * 100 - tens * 10;

            LEDBuffer[3] = mapDigit(thousands, false);
            LEDBuffer[2] = mapDigit(hundreds, false);
            LEDBuffer[1] = mapDigit(tens, false);
            LEDBuffer[0] = mapDigit(units, false);
        }

        else if (number > -100 && number < 0)
        {
            integerPart = (int32_t)floorf(-number);
            decimalPart = abs((int)((number + integerPart) * 10));
            tens = div10Approx(integerPart);
            units = integerPart - tens * 10;

            LEDBuffer[3] = tens == 0 ? NONE_TO_DISPLAY : MINUS_SIGN;
            LEDBuffer[2] = tens == 0 ? MINUS_SIGN : mapDigit(tens, false);
            LEDBuffer[1] = mapDigit(units, false);
            LEDBuffer[0] = mapDigit(decimalPart, true);
        }

        else if (number > 0 && number < 1000)
        {
            integerPart = (int32_t)floorf(number);
            decimalPart = (int)((number - integerPart) * 10);
            hundreds = div100Approx(integerPart);
            tens = div10Approx(integerPart - hundreds * 100);
            units = integerPart - hundreds * 100 - tens * 10;

            LEDBuffer[3] = hundreds == 0 ? NONE_TO_DISPLAY : mapDigit(hundreds, false);
            LEDBuffer[2] = tens == 0 && hundreds == 0 ? NONE_TO_DISPLAY : mapDigit(tens, false);
            LEDBuffer[1] = mapDigit(tens, false);
            LEDBuffer[0] = mapDigit(decimalPart, true);
        }

        else
            ;

        alpha4.clear();
        alpha4.writeDigitRaw(0, LEDBuffer[3]);
        alpha4.writeDigitRaw(1, LEDBuffer[2]);
        alpha4.writeDigitRaw(2, LEDBuffer[1]);
        alpha4.writeDigitRaw(3, LEDBuffer[0]);
        alpha4.writeDisplay();
    }
}