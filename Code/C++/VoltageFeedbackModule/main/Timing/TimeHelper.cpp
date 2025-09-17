#include "TimeHelper.hpp"
#include "esp_timer.h"

uint64_t TimeHelper::getTimeMilliseconds() {
    return esp_timer_get_time() / 1000;
}
