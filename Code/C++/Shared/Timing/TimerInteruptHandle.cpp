#include "TimerInterruptHandle.hpp"

TimerInterruptHandle::TimerInterruptHandle(esp_timer_handle_t timer)
    : timer_(timer)
{}

TimerInterruptHandle::TimerInterruptHandle(TimerInterruptHandle&& other) noexcept
    : timer_(other.timer_)
{
    other.timer_ = nullptr;
}

TimerInterruptHandle&
TimerInterruptHandle::operator=(TimerInterruptHandle&& other) noexcept
{
    if (this != &other) {
        cleanup();
        timer_ = other.timer_;
        other.timer_ = nullptr;
    }
    return *this;
}

TimerInterruptHandle::~TimerInterruptHandle()
{
    cleanup();
}

void TimerInterruptHandle::reset()
{
    cleanup();
}

void TimerInterruptHandle::cleanup() noexcept
{
    if (timer_) {
        esp_timer_stop(timer_);
        esp_timer_delete(timer_);
        timer_ = nullptr;
    }
}
