#pragma once

#include "../Logging/Log.hpp"
#include "../IO/Outputs.hpp"
#include <cstdlib>     // std::abort
#include <utility>     // std::forward

template<typename... Args>
[[noreturn]] void Aborter::safeAbort(const char* tag, const char* format, Args&&... args) {
    // 1) Put hardware in a safe state ASAP
    Outputs::toSafe();                 // should be noexcept in your design

    // 2) Log the fault (keep Log::Fatal non-throwing)
    Log::Fatal(tag, format, std::forward<Args>(args)...);

    // Optional: give UART a tick to flush, if needed (ESP-IDF)
    // vTaskDelay(pdMS_TO_TICKS(10));

    // 3) Hard stop (or esp_restart() if you prefer reboot)
    std::abort();
}
