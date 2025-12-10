#pragma once

#include <cstddef>
#include <cstdint>

class IChannel{
public:
    virtual ~IChannel() = default;

    virtual bool configure() = 0;
    virtual size_t readBytes(char* dst, size_t maxlen, uint32_t timeoutMs) = 0;
    virtual size_t writeBytes(const char* src, size_t len) = 0;
    virtual void flushTx() = 0;
    virtual const char* getDescription() const = 0;
};
