#pragma once

#include <cstdio>
#include <cstdarg>
#include "Macros/GetFileName.hpp"

class Log {
public:
    template<typename... Args>
    static void Error(const char* tag, const char* format, Args... args) {
        printColored("\033[31m", tag, format, args...); // Red
    }
    template<typename... Args>
	static void Error(const char* fileName, int lineNumber, const char* format, Args... args){
        printColored("\033[31m", fileName, lineNumber, format, args...); // Red
	}

    template<typename... Args>
    static void Info(const char* tag, const char* format, Args... args) {
        printColored("\033[32m", tag, format, args...); // Green
    }
    template<typename... Args>
	static void Info(const char* fileName, int lineNumber, const char* format, Args... args){
        printColored("\033[32m", fileName, lineNumber, format, args...); // Green
	}

    template<typename... Args>
    static void Debug(const char* tag, const char* format, Args... args) {
        printColored("\033[34m", tag, format, args...); // Blue
    }
    template<typename... Args>
	static void Debug(const char* fileName, int lineNumber, const char* format, Args... args){
        printColored("\033[34m", fileName, lineNumber, format, args...); // Blue
	}

    template<typename... Args>
    static void Warn(const char* tag, const char* format, Args... args) {
        printColored("\033[33m", tag, format, args...); // Yellow
    }
    template<typename... Args>
	static void Warn(const char* fileName, int lineNumber, const char* format, Args... args){
        printColored("\033[33m", fileName, lineNumber, format, args...); // Yellow
	}

    template<typename... Args>
    static void Fatal(const char* tag, const char* format, Args... args) {
        printColored("\033[35m", tag, format, args...); // Magenta
    }
    template<typename... Args>
	static void Fatal(const char* fileName, int lineNumber, const char* format, Args... args){
        printColored("\033[35m", fileName, lineNumber, format, args...); // Magenta
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
    template<typename... Args>
    static void printColored(const char* colorCode, const char* fileName, int lineNumber, const char* format, Args... args) {
        printf("%s[%s] line %d: ", colorCode, fileName, lineNumber);
        printf(format, args...);
        printf("\033[0m\n");
    }
};

#define LOG_ERROR(msg, ...) \
    Log::Error(GET_FILE_NAME, __LINE__, msg, ##__VA_ARGS__)


#define LOG_INFO(msg, ...) \
    Log::Info(GET_FILE_NAME, __LINE__, msg, ##__VA_ARGS__)

#define LOG_WARN(msg, ...) \
    Log::Warn(GET_FILE_NAME, __LINE__, msg, ##__VA_ARGS__)
	
#define LOG_DEBUG(msg, ...) \
    Log::Debug(GET_FILE_NAME, __LINE__, msg, ##__VA_ARGS__)

#define LOG_FATAL(msg, ...) \
    Log::Fatal(GET_FILE_NAME, __LINE__, msg, ##__VA_ARGS__)
