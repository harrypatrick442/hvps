#ifndef TIME_HELPER_HPP
#define TIME_HELPER_HPP
#include <cstdint> 
class TimeHelper {
public:
    // Returns the current time in milliseconds since boot
    static uint64_t s();
    static uint64_t us();
    static uint64_t ms();
};

#endif // TIME_HELPER_HPP
