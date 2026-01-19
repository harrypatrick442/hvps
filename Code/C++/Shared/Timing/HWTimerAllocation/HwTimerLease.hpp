#pragma once

#include "HwTimerConfig.hpp"
#include <cstdint>

class HwTimerLease {
public:
    const HwTimerConfig& config() const;

private:
    friend class HwTimerAllocator;

    HwTimerLease(const HwTimerConfig& cfg, uint8_t id);

    HwTimerConfig _config;
    uint8_t       _id;
};
