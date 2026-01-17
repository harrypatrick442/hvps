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
	static const char* TAG;
    static std::function<void()> _toSafe;
    static std::terminate_handler _oldTerminateHandler;
public:
	static void initialize(const std::function<void()>& toSafe);
	
    template<typename... Args>
    [[noreturn]] static void safeAbortFromMacro(
		const char* fileName, 
		int lineNumber,
		const char* msg,
		Args&&... args
	)
	{	
		if (_toSafe) {
			_toSafe();
		}	
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
    [[noreturn]] static inline void terminateHandler() noexcept;
	
};




