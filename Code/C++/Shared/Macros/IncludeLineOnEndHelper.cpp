#include "IncludeLineOnEnd.hpp"

std::string IncludeLineOnEndHelper::doIt(const char* msg, int line) {
    return doIt(std::string(msg), line);
}

std::string IncludeLineOnEndHelper::doIt(const std::string& msg, int line) {
    return msg + " line: " + std::to_string(line);
}
