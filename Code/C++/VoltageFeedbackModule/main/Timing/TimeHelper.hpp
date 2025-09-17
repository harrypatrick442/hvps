#ifndef TIME_HELPER_HPP
#define TIME_HELPER_HPP
#include <cstdint> 
class TimeHelper {
public:
    // Returns the current time in milliseconds since boot
    static uint64_t getTimeMilliseconds();
};

#endif // TIME_HELPER_HPP
