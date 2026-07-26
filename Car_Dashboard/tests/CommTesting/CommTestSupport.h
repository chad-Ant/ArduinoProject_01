#pragma once

#include "CommProtocolUnderTest.h"

// Kept in a header (not the .ino) so it is defined before Arduino's auto-generated
// function prototypes, which would otherwise reference it too early.
struct FeedResult {
    int     frames;
    bool    sawCrc;
    bool    sawOverflow;
    uint8_t lastType;
    uint8_t lastLen;
    uint8_t lastPayload[COMM_MAX_PAYLOAD];
};
