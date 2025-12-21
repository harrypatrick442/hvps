#pragma once
#include "Aborter.hpp"

#define __SAFE_ABORT__FILE_NAME__  (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#define SAFE_ABORT(msg, ...) \
    Aborter::safeAbortFromMacro(__SAFE_ABORT__FILE_NAME__, __LINE__)
