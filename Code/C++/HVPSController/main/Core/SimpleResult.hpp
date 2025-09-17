#ifndef SIMPLERESULT_HPP
#define SIMPLERESULT_HPP

#include <string>

struct SimpleResult {
    bool successful;
    std::string message;

    // Convenience factories
    static SimpleResult Success(const std::string& msg = "");
    static SimpleResult Failure(const std::string& msg = "");
};

#endif // SIMPLERESULT_HPP
