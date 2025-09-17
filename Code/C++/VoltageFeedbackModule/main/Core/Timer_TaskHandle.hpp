#pragma once
#include <functional>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <mutex>

class Timer_TaskHandle {
public:
    Timer_TaskHandle(Timer* timer);
    ~Timer_TaskHandle();

    void dispose();

private:
    bool _disposed;
};
