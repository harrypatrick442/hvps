#pragma once

#include "VoltageWithRaw.hpp"
#include <cstdint>

struct VoltageWithRawAndTime : public VoltageWithRaw
{
    uint64_t timeUs;  // Timestamp in microseconds

    VoltageWithRawAndTime(float voltageVal, uint16_t rawVal, uint64_t timeUsVal);
};
