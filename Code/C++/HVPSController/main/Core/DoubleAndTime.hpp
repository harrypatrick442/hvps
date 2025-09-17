#pragma once
#include <cstdint>

struct DoubleAndTime {
    double   d;
    uint64_t t;

    DoubleAndTime(double dIn, uint64_t tIn)
        : d(value), t(tIn) {}
};