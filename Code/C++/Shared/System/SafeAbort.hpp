#pragma once
#include "Aborter.hpp"
#include "Macros/GetFileName.hpp"
#define SAFE_ABORT(msg, ...) \
    Aborter::safeAbortFromMacro(GET_FILE_NAME, __LINE__, msg, ##__VA_ARGS__)
