#ifndef ABORTER_HPP
#define ABORTER_HPP

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
public:
	static inline void setToSafe(const std::function<void()>& fn) {
		_toSafe = fn;  // copy
	}
    template<typename... Args>
    [[noreturn]] static void safeAbort(const char* tag, const char* format, Args&&... args);
	static LastAbortMessage* getLastAbortReason(
			CleanupBucket& cleanupBucket);
	static bool hasLastAbortReason();
	static void clearLastAbortReason();
private:
    static inline std::function<void()> _toSafe = nullptr;
};

// Keep template implementation included
#include "Aborter.tpp"

// Header-safe terminate handler setup
namespace AborterDetail {
    [[noreturn]] inline void terminateHandler() noexcept {
        // No fmt args needed; keep it minimal to avoid any allocation here
        Aborter::safeAbort("Terminate", "Unhandled exception reached std::terminate()");
    }
}

inline void setupTerminateHandler() {
    std::set_terminate(AborterDetail::terminateHandler);
}

#endif // ABORTER_HPP
