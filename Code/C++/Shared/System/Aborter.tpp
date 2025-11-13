#include "../Logging/Log.hpp"
#include "../Storage/Flash.hpp"
#include "CrashReporter.hpp"  // for save_last_error_message
#include "esp_system.h"
#include <utility>
#include <cstdio>
#include <cstdarg>
#include "../Timing/Delay.hpp"
#include "BacktraceHelper.hpp"


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
	

    // 3) Log the fault normally (non-throwing)
    Log::Fatal(tag, "%s", formatted);
	
    constexpr int BACKTRACE_DEPTH = 16;
    uint32_t backtrace[BACKTRACE_DEPTH] = {0};
    size_t backtraceLength = BacktraceHelper::getBacktrace(
		backtrace, nullptr, BACKTRACE_DEPTH);
    // 4) Persist the formatted message to flash for post-mortem
	if(Flash::getIsInitialized()){
		Flash::setString(TAG, REASON_KEY,
			formatted);
		Flash::setArray(TAG, BACKTRACE_KEY,
			backtrace, backtraceLength);
	}
	else{
		Log::Warn(TAG, "Flash was not initialized when trying to set last abort reason");
	}
	
    // 5) Small delay to allow UART flush if needed
	Delay::ms(200);

    // 6) Restart system cleanly (don’t use std::abort here)
    esp_restart();

    // Should never reach here
    while (true) { }
}
