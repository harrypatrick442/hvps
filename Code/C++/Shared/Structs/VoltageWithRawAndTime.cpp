#include "VoltageWithRawAndTime.hpp"

VoltageWithRawAndTime::VoltageWithRawAndTime(float voltageVal, uint16_t rawVal, uint64_t timeUsVal)
    : VoltageWithRaw(voltageVal, rawVal), timeUs(timeUsVal)
{
}
