#pragma once

#include "../Logging/Log.hpp"
#include "CrashReporter.hpp"  // for save_last_error_message
#include "esp_system.h"
#include <utility>
#include <cstdio>
#include <cstdarg>


template<typename... Args>
[[noreturn]] void Aborter::safeAbort(const char* tag, const char* format, Args&&... args)
{
    // 1) Put hardware in a safe state ASAP
    if (_toSafe) {
        _toSafe();
    }

    // 2) Format the message for crash storage
    char formatted[128];//TODO long enough?
    std::snprintf(formatted, sizeof(formatted), format, std::forward<Args>(args)...);
	 // Copy to IRAM buffer for later retrieval (even if flash/heap are unavailable)
	std::strncpy(_reasonBuffer, formatted, sizeof(_reasonBuffer) - 1);
	_reasonBuffer[sizeof(_reasonBuffer) - 1] = '\0';
	_hasReason = true;

    // 3) Log the fault normally (non-throwing)
    Log::Fatal(tag, "%s", formatted);

    // 4) Persist the formatted message to flash for post-mortem
    //CrashReporter::saveLastErrorMessage("Aborter::safeAbort [%s]: %s", tag, formatted);

    // Optional: small delay to allow UART flush if needed
    // vTaskDelay(pdMS_TO_TICKS(10));

    // 5) Restart system cleanly (don’t use std::abort here)
    esp_restart();

    // Should never reach here
    while (true) { }
}
