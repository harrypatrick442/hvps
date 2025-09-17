#include "SimpleResult.hpp"

SimpleResult SimpleResult::Success(const std::string& msg) {
    return { true, msg };
}

SimpleResult SimpleResult::Failure(const std::string& msg) {
    return { false, msg };
}
