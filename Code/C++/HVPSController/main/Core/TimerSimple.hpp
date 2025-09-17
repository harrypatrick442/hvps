#pragma once
#include <functional>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <mutex>  
#include <atomic>
#include "RefCounted.hpp"

class TimerSimple : protected RefCounted{
public:
	static const char* TAG;
    using RefCounted::addRef;    // just these two become public
    using RefCounted::release;
    using Callback = std::function<void()> ;
    TimerSimple(uint32_t intervalMs, Callback callback, bool repeat = true);
    ~TimerSimple();
	bool start()noexcept;
    void dispose()noexcept;
	bool getIsRunning() const noexcept;

private:
    static void timerTask(void* param);

    uint32_t _intervalMs;
    Callback _callback;
    const bool _repeat;
    std::atomic<bool> _started{false};
    std::atomic<bool> _disposed{false};
    std::atomic<bool> _isRunning{false};
    TaskHandle_t _taskHandle{nullptr};
};
