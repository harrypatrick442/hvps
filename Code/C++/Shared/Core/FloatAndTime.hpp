#pragma once
#include <cstdint>

struct FloatAndTime {
    float f;
    uint64_t t;

    // default constructor
    constexpr FloatAndTime() noexcept : f(0.0), t(0) {}

    // parameterized constructor
    constexpr FloatAndTime(float fIn, uint64_t tIn) noexcept
        : f(fIn), t(tIn) {}
};