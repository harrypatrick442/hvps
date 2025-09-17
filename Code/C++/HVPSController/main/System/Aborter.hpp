#ifndef ABORTER_HPP
#define ABORTER_HPP

#include <exception>  // std::set_terminate

class Aborter {
public:
    template<typename... Args>
    [[noreturn]] static void safeAbort(const char* tag, const char* format, Args&&... args);
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
