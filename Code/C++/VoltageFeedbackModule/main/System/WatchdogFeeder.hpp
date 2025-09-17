#ifndef __WATCHDOGFEEDER_HPP
#define __WATCHDOGFEEDER_HPP
#include <cstdint>
#include <functional>  // Required for std::function
typedef std::function<void()> DelegateFeedWatchdog;
class WatchdogFeeder{
    public:
        static const char* TAG;
        static WatchdogFeeder& initialize(uint32_t timeout);
        static WatchdogFeeder& getInstance();
        void feed();
        DelegateFeedWatchdog getFeedWatchdog();
    private:
        static WatchdogFeeder* _instance;
        WatchdogFeeder();
        WatchdogFeeder(const WatchdogFeeder&) = delete;
        WatchdogFeeder& operator=(const WatchdogFeeder&) = delete;
        const char* getMovingPrey();
        void huntForFood();
        uint64_t _successfulHunts;
        uint8_t _lastKillingWasIndex;
};
#endif // __WATCHDOGFEEDER_HPP