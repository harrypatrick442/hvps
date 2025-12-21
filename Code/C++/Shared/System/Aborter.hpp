#pragma once

#include <exception>  // std::set_terminate
#include <functional>  // std::function
#include <utility>     // std::forward
#include "Aborter.hpp"
#include "esp_attr.h"
#include <string>
#include "../Generated/Messages/LastAbortMessage.hpp"
#include "../Core/CleanupBucket.hpp"
class Aborter {
private: 
	static inline constexpr const char* TAG = "Aborter";
	static inline constexpr const char*  REASON_KEY = "reason";
	static inline constexpr const char*  BACKTRACE_KEY = "bt";
    static inline std::function<void()> _toSafe = nullptr;
	
public:
	static inline void setToSafe(const std::function<void()>& fn) {
		_toSafe = fn;
	}
	
    //template<typename... Args>
    [[noreturn]] static void safeAbortFromMacro(
		const char* fileName,
		int lineNumber
		//, Args&&... args
	);
	
	static LastAbortMessage* getLastAbortReason(
			CleanupBucket& cleanupBucket);
	static bool hasLastAbortReason();
	static void clearLastAbortReason();
	
private:
	[[noreturn]] void _safeAbort(const char* fileName, char* formatted);
	
};

// Header-safe terminate handler setup
namespace AborterDetail {
    [[noreturn]] inline void terminateHandler() noexcept {
        // No fmt args needed; keep it minimal to avoid any allocation here
		
		// 1) Put hardware in a safe state ASAP
		if (_toSafe) {
			_toSafe();
		}

		// 2) Format the message for crash storage
		char formatted[128];//TODO long enough?
		std::snprintf(formatted, sizeof(formatted), format, std::forward<Args>(args)...);
		_safeAbort(formatted);
        Aborter::safeAbort("Terminate", "Unhandled exception reached std::terminate()");
    }
}

inline void setupTerminateHandler() {
    std::set_terminate(AborterDetail::terminateHandler);
}
