#include "WatchdogsAndSize.hpp"
#include "Watchdog.hpp"

WatchdogsAndSize::WatchdogsAndSize(
    const std::vector<std::shared_ptr<Watchdog>>& source)
    : entries([&]() {
          auto ptrs = new Watchdog*[source.size()];
          for (size_t i = 0; i < source.size(); ++i) {
              ptrs[i] = source[i].get();
          }
          return ptrs;
      }()),
      size(source.size())
{
}

WatchdogsAndSize::~WatchdogsAndSize()
{
    delete[] entries;
}
