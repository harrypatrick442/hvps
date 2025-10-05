#include "TicketSource.hpp"

std::mutex  TicketSource::_mutex;
uint64_t    TicketSource::_nextTicket = 1;

uint64_t TicketSource::next() noexcept {
    std::lock_guard<std::mutex> lock(_mutex);

    uint64_t ticket = _nextTicket;
    if (_nextTicket >= MAX_SAFE) {
        _nextTicket = 1; // wrap
    } else {
        ++_nextTicket;
    }
    return ticket; // always in [0, MAX_SAFE]
}
