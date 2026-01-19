#include "HwTimerAllocator.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

namespace
{
    constexpr uint8_t TIMER_COUNT = 4;

    constexpr HwTimerConfig TIMER_MAP[TIMER_COUNT] = {
        { TIMER_GROUP_0, TIMER_0 },
        { TIMER_GROUP_0, TIMER_1 },
        { TIMER_GROUP_1, TIMER_0 },
        { TIMER_GROUP_1, TIMER_1 }
    };

    bool        s_inUse[TIMER_COUNT] = {};
    const char* s_owner[TIMER_COUNT] = {};

    // Fixed, eagerly-created mutex — no races, no lazy init
    SemaphoreHandle_t s_mutex = xSemaphoreCreateMutex();
}

std::unique_ptr<HwTimerLease> HwTimerAllocator::acquire(const char* owner)
{
    xSemaphoreTake(s_mutex, portMAX_DELAY);

    for (uint8_t i = 0; i < TIMER_COUNT; ++i) {
        if (!s_inUse[i]) {
            s_inUse[i] = true;
            s_owner[i] = owner;

            xSemaphoreGive(s_mutex);

            auto deleter = [](HwTimerLease* lease) {
                HwTimerAllocator::release(lease->_id);
                delete lease;
            };

            return std::unique_ptr<HwTimerLease>(
                new HwTimerLease(TIMER_MAP[i], i),
                deleter
            );
        }
    }

    xSemaphoreGive(s_mutex);
    abort("No hardware timers available", owner, -1);
    __builtin_unreachable();
}

void HwTimerAllocator::release(uint8_t id)
{
    xSemaphoreTake(s_mutex, portMAX_DELAY);

    if (id >= TIMER_COUNT || !s_inUse[id]) {
        xSemaphoreGive(s_mutex);
        abort("Invalid or double timer release", nullptr, id);
    }

    s_inUse[id] = false;
    s_owner[id] = nullptr;

    xSemaphoreGive(s_mutex);
}
