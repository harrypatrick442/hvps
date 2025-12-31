#pragma once

#include <exception>  // std::set_terminate
#include <functional>  // std::function
#include <utility>     // std::forward
#include "Aborter.hpp"
#include "esp_attr.h"
#include <string>
#include "../Generated/Messages/LastAbortMessage.hpp"
#include "../Core/CleanupBucket.hpp"
#include "BacktraceHelper.hpp"
class Aborter {
private:
	static const char*  REASON_KEY;
	static const char*  BACKTRACE_KEY;
    static inline std::function<void()> _toSafe = nullptr;
	static const char* TAG;
	
public:
	static inline void setToSafe(const std::function<void()>& fn) {
		_toSafe = fn;
	}
	
    template<typename... Args>
    [[noreturn]] static void safeAbortFromMacro(
		const char* fileName, 
		int lineNumber,
		const char* msg,
		Args&&... args
	)
	{		
		char message[256];
		std::snprintf(message, sizeof(message), msg, std::forward<Args>(args)...);
		_safeAbort(fileName, lineNumber, message);
	}
	
	static LastAbortMessage* getLastAbortReason(
			CleanupBucket& cleanupBucket);
	static bool hasLastAbortReason();
	static void clearLastAbortReason();
	
private:
	[[noreturn]] static void _safeAbort(const char* fileName, int lineNumber, char* message);
	[[noreturn]] static void _safeAbort(const char* fileName, char* formatted);
	
};

// Header-safe terminate handler setup
/*
namespace AborterDetail {
    [[noreturn]] inline void terminateHandler() noexcept {
        // No fmt args needed; keep it minimal to avoid any allocation here
		
		// 1) Put hardware in a safe state ASAP
		if (_toSafe) {
			_toSafe();
		}	
		Aborter::_safeAbort("Aborter.hpp", "abort happened");
    }
}
inline void setupTerminateHandler() {
    std::set_terminate(AborterDetail::terminateHandler);
}*/

