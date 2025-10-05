#include "Latch.hpp"

Latch::Latch(bool latched)
{
    eg_ = xEventGroupCreate();
    configASSERT(eg_ != nullptr);

    if (latched) {
        // start blocking: ensure bit is clear
        xEventGroupClearBits(eg_, UNLATCHED_BIT);
    } else {
        // start released: set bit so waiters pass immediately
        xEventGroupSetBits(eg_, UNLATCHED_BIT);
    }
}

Latch::~Latch()
{
    if (eg_) {
        vEventGroupDelete(eg_);
        eg_ = nullptr;
    }
}

void Latch::latch()
{
    // Return to blocking state (future waiters will block).
    // Note: tasks that have *already* passed won't be affected.
    xEventGroupClearBits(eg_, UNLATCHED_BIT);
}

void Latch::unlatch()
{
    // Release all waiters and keep released for future waiters (manual-reset style).
    xEventGroupSetBits(eg_, UNLATCHED_BIT);
}

void Latch::wait() const
{
    // Wait until the UNLATCHED_BIT is observed set. Do not clear it on exit.
    (void)xEventGroupWaitBits(
        eg_, UNLATCHED_BIT,
        pdFALSE,      // do not clear on exit (manual-reset behavior)
        pdTRUE,       // wait for all bits (just this one)
        portMAX_DELAY // block forever
    );
}

bool Latch::wait(uint32_t timeout_ms) const
{
    EventBits_t bits = xEventGroupWaitBits(
        eg_, UNLATCHED_BIT,
        pdFALSE,                         // keep set so others pass too
        pdTRUE,
        pdMS_TO_TICKS(timeout_ms)
    );
    return (bits & UNLATCHED_BIT) != 0;
}

bool Latch::isLatched() const
{
    EventBits_t bits = xEventGroupGetBits(eg_);
    return (bits & UNLATCHED_BIT) == 0;
}
