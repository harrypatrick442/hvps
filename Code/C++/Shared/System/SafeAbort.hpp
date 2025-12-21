#pragma once
#include "Aborter.hpp"
#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#define SAFE_ABORT(msg, ...) \
    Aborter::safeAbortFromMacro(__FILENAME__, __LINE__)
//#define SAFE_ABORT(msg, ...) \
//    Aborter::safeAbort(Hash::fileNameHash(__FILE__), __LINE__, ##__VA_ARGS__)