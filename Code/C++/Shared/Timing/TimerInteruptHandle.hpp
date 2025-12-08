#pragma once
#include "esp_timer.h"

// RAII wrapper that automatically stops and deletes an ESP timer
// when the object is destroyed.
class TimerInterruptHandle
{
public:
    explicit TimerInterruptHandle(esp_timer_handle_t timer);

    // Non-copyable
    TimerInterruptHandle(const TimerInterruptHandle&) = delete;
    TimerInterruptHandle& operator=(const TimerInterruptHandle&) = delete;

    // Movable
    TimerInterruptHandle(TimerInterruptHandle&& other) noexcept;
    TimerInterruptHandle& operator=(TimerInterruptHandle&& other) noexcept;

    ~TimerInterruptHandle();

    // Optional early cleanup
    void reset();

    // Check if valid
    bool valid() const { return timer_ != nullptr; }

private:
    esp_timer_handle_t timer_{nullptr};

    void cleanup() noexcept;
};
