#include "WatchdogsAndSize.hpp"
#include "Watchdog.hpp"

WatchdogsAndSize::WatchdogsAndSize(
    const std::vector<std::shared_ptr<Watchdog>>& source)
    : entries(nullptr),
      size(source.size())
{
    const Watchdog** ptrs = new const Watchdog*[size];
    for (size_t i = 0; i < size; ++i) {
        ptrs[i] = source[i].get();
    }
    entries = ptrs;
}

WatchdogsAndSize::~WatchdogsAndSize()
{
    delete[] entries;
}

WatchdogsAndSize::WatchdogsAndSize(WatchdogsAndSize&& other) noexcept
    : entries(other.entries),
      size(other.size)
{
    other.entries = nullptr;
}

WatchdogsAndSize&
WatchdogsAndSize::operator=(WatchdogsAndSize&& other) noexcept
{
    if (this != &other) {
        delete[] entries;
        entries = other.entries;
        other.entries = nullptr;
        // size is const by design → cannot be reassigned
    }
    return *this;
}
