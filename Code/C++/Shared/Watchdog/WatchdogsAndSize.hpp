#pragma once

#include <vector>
#include <memory>

class Watchdog;

struct WatchdogsAndSize {
public:
    const Watchdog* const* entries;
    const size_t size;

    explicit WatchdogsAndSize(
        const std::vector<std::shared_ptr<Watchdog>>& source);

    ~WatchdogsAndSize();

    // non-copyable
    WatchdogsAndSize(const WatchdogsAndSize&) = delete;
    WatchdogsAndSize& operator=(const WatchdogsAndSize&) = delete;

    // movable
    WatchdogsAndSize(WatchdogsAndSize&& other) noexcept;
    WatchdogsAndSize& operator=(WatchdogsAndSize&& other) noexcept;
};
