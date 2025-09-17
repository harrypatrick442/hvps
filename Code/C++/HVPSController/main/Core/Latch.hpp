#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

class Latch {
public:
    // latched=true  -> start in blocking state
    // latched=false -> start released (non-blocking)
    explicit Latch(bool latched = true);
    ~Latch();

    // Non-copyable / non-movable (owns RTOS handles)
    Latch(const Latch&) = delete;
    Latch& operator=(const Latch&) = delete;
    Latch(Latch&&) = delete;
    Latch& operator=(Latch&&) = delete;

    // Make the latch blocking (future waiters will block)
    void latch();

    // Release the latch (all current/future waiters pass)
    void unlatch();

    // Block until unlatched
    void wait() const;

    // Block until unlatched or timeout (ms). Returns true if unlatched observed.
    bool wait(uint32_t timeout_ms) const;

    // Query state: true if currently blocking
    bool isLatched() const;

private:
    EventGroupHandle_t eg_;
    static constexpr EventBits_t UNLATCHED_BIT = BIT0;
};
