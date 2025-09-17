#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <mutex>
#include <functional>
#include "TimerSimple.hpp"

class Timer{
public:
	static const char* TAG;
    using Callback = std::function<void()>;

    Timer(uint32_t intervalMs, Callback cb, bool repeat = true);
    ~Timer();

    void start()noexcept;
    void stop()noexcept;
    bool isRunning() const noexcept;

private:
    uint32_t _intervalMs;
    Callback _callback;
    const bool _repeat;

	mutable std::mutex _mutex;
    mutable TimerSimple* _current;
};
