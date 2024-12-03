#include "../include/SegmentLEDFunctions.h"
#include "../include/QuickMathFunctions.h"

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

DigitMapping mapString(char c)
{
    switch (c)
    {
    case '0':
        return NUM_0_MIRROR;
    case '1':
        return NUM_1_MIRROR;
    case '2':
        return NUM_2_MIRROR;
    case '3':
        return NUM_3_MIRROR;
    case '4':
        return NUM_4_MIRROR;
    case '5':
        return NUM_5_MIRROR;
    case '6':
        return NUM_6_MIRROR;
    case '7':
        return NUM_7_MIRROR;
    case '8':
        return NUM_8_MIRROR;
    case '9':
        return NUM_9_MIRROR;
    case 'A':
    case 'a':
        return CHAR_A_MIRROR;
    case 'B':
    case 'b':
        return CHAR_B_MIRROR;
    case 'C':
    case 'c':
        return CHAR_C_MIRROR;
    case 'D':
    case 'd':
        return CHAR_D_MIRROR;
    case 'E':
    case 'e':
        return CHAR_E_MIRROR;
    case 'F':
    case 'f':
        return CHAR_F_MIRROR;
    case 'G':
    case 'g':
        return CHAR_G_MIRROR;
    case 'H':
    case 'h':
        return CHAR_H_MIRROR;
    case 'I':
    case 'i':
        return CHAR_I_MIRROR;
    case 'J':
    case 'j':
        return CHAR_J_MIRROR;
    case 'K':
    case 'k':
        return CHAR_K_MIRROR;
    case 'L':
    case 'l':
        return CHAR_L_MIRROR;
    case 'M':
    case 'm':
        return CHAR_M_MIRROR;
    case 'N':
    case 'n':
        return CHAR_N_MIRROR;
    case 'O':
    case 'o':
        return CHAR_O_MIRROR;
    case 'P':
    case 'p':
        return CHAR_P_MIRROR;
    case 'Q':
    case 'q':
        return CHAR_Q_MIRROR;
    case 'R':
    case 'r':
        return CHAR_R_MIRROR;
    case 'S':
    case 's':
        return CHAR_S_MIRROR;
    case 'T':
    case 't':
        return CHAR_T_MIRROR;
    case 'U':
    case 'u':
        return CHAR_U_MIRROR;
    case 'V':
    case 'v':
        return CHAR_V_MIRROR;
    case 'W':
    case 'w':
        return CHAR_W_MIRROR;
    case 'X':
    case 'x':
        return CHAR_X_MIRROR;
    case 'Y':
    case 'y':
        return CHAR_Y_MIRROR;
    case 'Z':
    case 'z':
        return CHAR_Z_MIRROR;
    case '-':
        return MINUS_SIGN;
    case '+':
        return PLUS_CROSS;
    case '*':
        return STAR_5;
    case '/':
        return FSLASH_MIRROR;
    case '\\':
        return BSLASH_MIRROR;
    case ' ':
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

void loadingAnimation(Adafruit_AlphaNum4 &alpha4){
    static uint8_t currentPosition = 1;
    static uint16_t currentSegment = 0x1;
    
}

void writeFloatLED_Mirror(Adafruit_AlphaNum4 &alpha4,float number)
{
    DigitMapping LEDBuffer[4] = {
        NONE_TO_DISPLAY,
        NONE_TO_DISPLAY,
        NONE_TO_DISPLAY,
        NONE_TO_DISPLAY};
    int integerPart = 0;
    int decimalPart = 0;
    int units = 0;
    int tens = 0;
    int hundreds = 0;
    int64_t thousands = 0;

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
    else if (number == 0){
        LEDBuffer[0] = NUM_0_DP_MIRROR;
        LEDBuffer[1] = NUM_0_MIRROR;
    }
    else
    {
        number = number >= -999 ? (number <= 9999 ? number : 9999) : -999;

        if (number > 0 && number < 1000)
        {
            integerPart = (int32_t)(floorf(number));
            decimalPart = (int32_t)((number - integerPart) * 10);
            hundreds = div100Approx(integerPart);
            tens = div10Approx(integerPart - hundreds * 100);
            units = integerPart - hundreds * 100 - tens * 10;

            LEDBuffer[3] = hundreds == 0 ? NONE_TO_DISPLAY : mapDigit(hundreds, false);
            LEDBuffer[2] = tens == 0 && hundreds == 0 ? NONE_TO_DISPLAY : mapDigit(tens, false);
            LEDBuffer[1] = mapDigit(units, false);
            LEDBuffer[0] = mapDigit(decimalPart, true);
        }

        else if (number > -100 && number < 0)
        {
            integerPart = (int32_t)(floorf(-number));
            decimalPart = abs((int)((number + integerPart) * 10));
            tens = div10Approx(integerPart);
            units = integerPart - tens * 10;
            
            LEDBuffer[3] = tens == 0 ? NONE_TO_DISPLAY : MINUS_SIGN;
            LEDBuffer[2] = tens == 0 ? MINUS_SIGN : mapDigit(tens, false);
            LEDBuffer[1] = mapDigit(units, false);
            LEDBuffer[0] = mapDigit(decimalPart, true);
        }

        else if (number <= -100)
        {
            integerPart = abs((int32_t)number);
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
            integerPart = (int32_t)number;
            thousands = div1000Approx(integerPart);
            hundreds = div100Approx(integerPart - thousands * 1000);
            tens = div10Approx(integerPart - thousands * 1000 - hundreds * 100);
            units = integerPart - thousands * 1000 - hundreds * 100 - tens * 10;

            LEDBuffer[3] = mapDigit(thousands, false);
            LEDBuffer[2] = mapDigit(hundreds, false);
            LEDBuffer[1] = mapDigit(tens, false);
            LEDBuffer[0] = mapDigit(units, false);
        }

        else
            ;
    }

    alpha4.clear();
    alpha4.writeDigitRaw(0, LEDBuffer[0]);
    alpha4.writeDigitRaw(1, LEDBuffer[1]);
    alpha4.writeDigitRaw(2, LEDBuffer[2]);
    alpha4.writeDigitRaw(3, LEDBuffer[3]);
    alpha4.writeDisplay();
    return;
}

void writeStringLED_Mirror(Adafruit_AlphaNum4 &alpha4, const char *stringInput)
{
    /*!Only show first 4 chars*/
    if (stringInput == nullptr)
    {
        alpha4.clear();
        alpha4.writeDigitRaw(0, NONE_TO_DISPLAY);
        alpha4.writeDigitRaw(1, NONE_TO_DISPLAY);
        alpha4.writeDigitRaw(2, NONE_TO_DISPLAY);
        alpha4.writeDigitRaw(3, NONE_TO_DISPLAY);
        alpha4.writeDisplay();
        return;
    }

    static DigitMapping LEDBuffer[4] = {
        NONE_TO_DISPLAY,
        NONE_TO_DISPLAY,
        NONE_TO_DISPLAY,
        NONE_TO_DISPLAY};
    int scrollIndex = 0;
    int stringLength = strlen(stringInput);

    for (int i = 0; i < 4; i++)
    {
        LEDBuffer[3 - i] = mapString(stringInput[i]);
    }

    alpha4.clear();
    alpha4.writeDigitRaw(0, LEDBuffer[0]);
    alpha4.writeDigitRaw(1, LEDBuffer[1]);
    alpha4.writeDigitRaw(2, LEDBuffer[2]);
    alpha4.writeDigitRaw(3, LEDBuffer[3]);
    alpha4.writeDisplay();
}