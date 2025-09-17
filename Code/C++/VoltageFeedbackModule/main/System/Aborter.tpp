#pragma once

#include "../Logging/Log.hpp"
#include "../IO/Outputs.hpp"  // wherever Outputs::toSafe() lives
#include <cstdlib>              // abort()

template<typename... Args>
void Aborter::safeAbort(const char* tag, const char* format, Args... args) {
    Outputs::toSafe();  // FIRST, disable outputs safely
    Log::Fatal(tag, format, args...);  // THEN, log error
    abort();  // Finally, halt the system
}
