#ifndef ABORTER_HPP
#define ABORTER_HPP

class Aborter {
public:
    template<typename... Args>
    static void safeAbort(const char* tag, const char* format, Args... args);
};

#include "Aborter.tpp" // Include implementation for templates

#endif // ABORTER_HPP
