#pragma once
#include <string>

class IncludeLineOnEndHelper {
public:
    static std::string doIt(const char* msg, int line);
    static std::string doIt(const std::string& msg, int line);
};

#define INCLUDE_LINE_ON_END(msg) \
    IncludeLineOnEndHelper::doIt((msg), __LINE__)
