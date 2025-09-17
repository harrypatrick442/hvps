#pragma once
#ifndef CANCELLATION_TOKEN_HPP
#define CANCELLATION_TOKEN_HPP

#include <functional>
#include <vector>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "RefCounted.hpp"

class CancellationToken : protected RefCounted {
public:
    using RefCounted::addRef;    // just these two become public
    using RefCounted::release;
    explicit CancellationToken(bool initiallyCancelled = false);
    ~CancellationToken();

    // Control
    void cancel();                          // signal cancellation
    bool isCancellationRequested() const;   // query state

    // Waiting
    void wait() const;                      // block until cancelled
    bool wait(uint32_t timeout_ms) const;   // block until cancelled or timeout

    // Callbacks
    int reg(std::function<void()> cb); // register a callback
    void deregister(int handle);            // deregister by handle

    // No copying
    CancellationToken(const CancellationToken&) = delete;
    CancellationToken& operator=(const CancellationToken&) = delete;

private:
    EventGroupHandle_t  _eventGroupHandle;
    SemaphoreHandle_t   _mutex;
    std::vector<std::function<void()>> _callbacks;

    static constexpr EventBits_t _cancelledBIT = BIT0;
};

#endif // CANCELLATION_TOKEN_HPP
