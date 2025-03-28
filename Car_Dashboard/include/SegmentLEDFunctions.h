#ifndef SEGMENT_LED_FUNCTIONS
#define SEGMENT_LED_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

//there isn't much get functions in the Adafruit_AlphaNum4 library, so debug code will be limited

enum DigitMapping
{   
    NUM_0 = 0x213F,
    NUM_0_DP = 0x613F,
    NUM_1 = 0x406,
    NUM_1_DP = 0x4406,
    NUM_1_MIRROR = 0x130,
    NUM_1_DP_MIRROR = 0x4130,
    NUM_2 = 0x88B,
    NUM_2_DP = 0x488B,
    NUM_2_MIRROR = 0x2069,
    NUM_2_DP_MIRROR = 0x6069,
    NUM_3 = 0x8F,
    NUM_3_DP = 0x408F,
    NUM_3_MIRROR = 0x79,
    NUM_3_DP_MIRROR = 0x4079,
    NUM_4 = 0xE6,
    NUM_4_DP = 0x40E6,
    NUM_4_MIRROR = 0xF2,
    NUM_4_DP_MIRROR = 0x40F2,
    NUM_5 = 0x2069,
    NUM_5_DP = 0x6069,
    NUM_5_MIRROR = 0x88B,
    NUM_5_DP_MIRROR = 0x488B,
    NUM_6 = 0xFD,
    NUM_6_DP = 0x40FD,
    NUM_6_MIRROR = 0xDF,
    NUM_6_DP_MIRROR = 0x40DF,
    NUM_7 = 0x7,
    NUM_7_DP = 0x4007,
    NUM_7_MIRROR = 0x31,
    NUM_7_DP_MIRROR = 0x4031,
    NUM_8 = 0xFF,
    NUM_8_DP = 0x40FF,
    NUM_9 = 0xEF,
    NUM_9_DP = 0x40EF,
    NUM_9_MIRROR = 0xFB,
    NUM_9_DP_MIRROR = 0x40FB,
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
    CHAR_HYPHEN = 0x200,
    ALL_SEGMENTS = 0x7FFF,
    X_CROSS = 0x2D00,
    PLUS_CROSS = 0x12C0,
    STAR_5 = 0x2AC0,
    STAR_8 = 0x3FC0,
    EXPONENT_E_MIRROR = 0x208C,
    MINUS_SIGN = 0x40,
    EQUAL_SIGN = 0xC8,
    GREATER_SIGN_MIRROR = 0x2400,
    GEQ_SIGN_MIRROR = 0x2408,
    LESS_SIGN_MIRROR = 0x900,
    LEQ_SIGN_MIRROR = 0x908,
    LEFT_ARROW1_MIRROR = 0x940,
    LEFT_ARROW2_MIRROR = 0x9C0,
    RIGHT_ARROW1_MIRROR = 0x2480,
    RIGHT_ARROW2_MIRROR = 0x24C0,
    UP_ARROW1 = 0x3800,
    UP_ARROW2 = 0x3A00,
    DOWN_ARROW1 = 0x700,
    DOWN_ARROW2 = 0x1700,
    DASH = 0xC0,
    UNDERSCORE = 0x8,
    COMMA_MIRROR = 0x2000,
    FSLASH_MIRROR = 0x2040,
    BSLASH_MIRROR = 0xC00,
    NONE_TO_DISPLAY = 0x0
};

//typedef uint64_t LEDFrame; 
//bit 0-15 -> LED 0, bit 16-31 -> LED 1, bit 32-47 -> LED 2, bit 48-63 -> LED 3

bool initializeSegmentLED(Adafruit_AlphaNum4 &alpha4);
void clearSegmentLED(Adafruit_AlphaNum4 &alpha4);
void adjustLEDBrightness(Adafruit_AlphaNum4 &alpha4, uint8_t ambientLuminosity);
void writeFloatLED_Mirror(Adafruit_AlphaNum4 &alpha4, float number);
void writeStringLED_Mirror(Adafruit_AlphaNum4 &alpha4, const char *stringInput);
void writeFloatLED(Adafruit_AlphaNum4 &alpha4, float number);
void writeStringLED(Adafruit_AlphaNum4 &alpha4, const char *stringInput);
#endif