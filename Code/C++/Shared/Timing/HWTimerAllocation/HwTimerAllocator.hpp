#pragma once

#include "HwTimerLease.hpp"
#include <memory>
#include <cstdint>

class HwTimerAllocator {
public:

    // Acquire an exclusive hardware timer.
    // Aborts if none are available.
    static LeasePtr acquire(const char* owner);

private:
    friend class HwTimerLease;

    static void release(uint8_t id);

    static void abort(const char* reason, const char* owner, int id);
};
