#pragma once
#include "driver/timer.h"
#include <functional>

class HardwareTimerLease {
private:
    timer_group_t _group;
    timer_idx_t   _index;
    bool _released;
    std::function<void()> _releaseFunc;

public:
    HardwareTimerLease(
        timer_group_t group,
        timer_idx_t index,
        std::function<void()> releaseFunc
    );

    ~HardwareTimerLease();

    timer_group_t getGroup() const;
    timer_idx_t   getIndex() const;

    // Non-copyable
    HardwareTimerLease(const HardwareTimerLease&) = delete;
    HardwareTimerLease& operator=(const HardwareTimerLease&) = delete;

    // Movable (optional, but safe). I chose to take it out as not required with unique_ptr
    HardwareTimerLease(HardwareTimerLease&& other) = delete;
    HardwareTimerLease& operator=(HardwareTimerLease&& other) = delete;
};
