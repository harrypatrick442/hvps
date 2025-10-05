#pragma once

#include <cstdint>

class Delay {
public:
    static void ms(uint32_t milliseconds);
    static void ticks(uint32_t ticks);
};
