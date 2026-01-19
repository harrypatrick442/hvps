#pragma once

#include "driver/timer.h"
struct HwTimerConfig {
    timer_group_t group;
    timer_idx_t   index;
};
