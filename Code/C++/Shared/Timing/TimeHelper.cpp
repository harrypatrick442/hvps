#include "TimeHelper.hpp"
#include "esp_timer.h"

uint64_t TimeHelper::s() {
    return esp_timer_get_time() / 1000000;
}
uint64_t TimeHelper::ms() {
    return esp_timer_get_time() / 1000;
}
uint64_t TimeHelper::us() {
    return esp_timer_get_time();
}
