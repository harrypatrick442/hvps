#pragma once

#include <vector>
#include <memory>

class Watchdog;

struct WatchdogsAndSize {
public:
    Watchdog* const* const entries;
    const size_t size;

    explicit WatchdogsAndSize(
        const std::vector<std::shared_ptr<Watchdog>>& source);

    ~WatchdogsAndSize();

    // non-copyable
    WatchdogsAndSize(const WatchdogsAndSize&) = delete;
    WatchdogsAndSize& operator=(const WatchdogsAndSize&) = delete;

    // non-movable
    WatchdogsAndSize(WatchdogsAndSize&& other) = delete;
    WatchdogsAndSize& operator=(WatchdogsAndSize&& other) = delete;
};
