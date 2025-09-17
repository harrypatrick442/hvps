#pragma once
#include <cstdint>
#include <mutex>

struct TicketSource {
    static constexpr uint64_t MAX_SAFE = (1ULL << 53) - 1; // 9,007,199,254,740,991

    static uint64_t next() noexcept;

private:
    static std::mutex _mutex;
    static uint64_t _nextTicket; // always in [0, MAX_SAFE]
};
