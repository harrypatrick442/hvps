#pragma once

#include <functional>
#include <cstdint>
#include "esp_timer.h"
#include "TimerInterruptHandle.hpp"

class TimerInterruptHelper
{
public:
    static TimerInterruptHandle usingTimerInterrupt(
        uint64_t periodUs,
        esp_timer_cb_t isr,
        void* argumentForIsr = nullptr,
		const char* name = "temp_timer");
};
