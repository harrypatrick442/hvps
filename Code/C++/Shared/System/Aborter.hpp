#ifndef ABORTER_HPP
#define ABORTER_HPP

#include <exception>  // std::set_terminate
#include <functional>  // std::function
#include <utility>     // std::forward
#include "Aborter.hpp"
#include "esp_attr.h"
class Aborter {
private: 
    static IRAM_ATTR char _reasonBuffer[128];
    static IRAM_ATTR bool _hasReason;
public:
	static inline void setToSafe(const std::function<void()>& fn) {
		_toSafe = fn;  // copy
	}
    template<typename... Args>
    [[noreturn]] static void safeAbort(const char* tag, const char* format, Args&&... args);
	static const char* getLastAbortReason();
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
