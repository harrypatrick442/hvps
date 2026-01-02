#pragma once

#include <cstdint>

struct VoltageWithRaw
{
    float voltage;      // Measured voltage in volts (e.g., 3.30f)
    uint16_t raw;       // Raw ADC reading (0–4095 for 12-bit)
    VoltageWithRaw(float voltage, uint16_t raw);
};