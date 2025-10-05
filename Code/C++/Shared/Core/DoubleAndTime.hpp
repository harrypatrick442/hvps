#pragma once
#include <cstdint>

struct DoubleAndTime {
    double   d;
    uint64_t t;

    // default constructor
    constexpr DoubleAndTime() noexcept : d(0.0), t(0) {}

    // parameterized constructor
    constexpr DoubleAndTime(double dIn, uint64_t tIn) noexcept
        : d(dIn), t(tIn) {}
};