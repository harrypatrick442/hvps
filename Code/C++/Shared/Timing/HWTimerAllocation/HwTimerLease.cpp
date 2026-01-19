#include "HwTimerLease.hpp"

HwTimerLease::HwTimerLease(const HwTimerConfig& cfg, uint8_t id)
    : _config(cfg)
    , _id(id)
{
}

const HwTimerConfig& HwTimerLease::config() const
{
    return _config;
}
