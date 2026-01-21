#pragma once

#include "HardwareTimerInfo.hpp"
#include "HardwareTimerLease.hpp"

#include <memory>
#include <mutex>
#include <cstddef>
#include <string>

class HardwareTimerPool {
private:
    static constexpr std::size_t N_INFOS = 4;
    static HardwareTimerInfo _timerInfos[N_INFOS];
    static std::mutex _mutex;

public:
    static std::unique_ptr<HardwareTimerLease> acquire(std::string ownerName);
};
