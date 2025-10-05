#ifndef LOG_HPP
#define LOG_HPP

#include <cstdio>
#include <cstdarg>

class Log {
public:
    template<typename... Args>
    static void Error(const char* tag, const char* format, Args... args) {
        printColored("\033[31m", tag, format, args...); // Red
    }

    template<typename... Args>
    static void Info(const char* tag, const char* format, Args... args) {
        printColored("\033[32m", tag, format, args...); // Green
    }

    template<typename... Args>
    static void Debug(const char* tag, const char* format, Args... args) {
        printColored("\033[34m", tag, format, args...); // Blue
    }

    template<typename... Args>
    static void Warn(const char* tag, const char* format, Args... args) {
        printColored("\033[33m", tag, format, args...); // Yellow
    }

    template<typename... Args>
    static void Fatal(const char* tag, const char* format, Args... args) {
        printColored("\033[35m", tag, format, args...); // Magenta
    }

    template<typename... Args>
    static void Trace(const char* tag, const char* format, Args... args) {
        printColored("\033[36m", tag, format, args...); // Cyan
    }

private:
    template<typename... Args>
    static void printColored(const char* colorCode, const char* tag, const char* format, Args... args) {
        printf("%s[%s] ", colorCode, tag);
        printf(format, args...);
        printf("\033[0m\n");
    }
};

#endif // LOG_HPP