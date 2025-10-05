#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Delay.hpp"

void Delay::ms(uint32_t milliseconds) {
    vTaskDelay(pdMS_TO_TICKS(milliseconds));
}

void Delay::ticks(uint32_t ticks) {
    vTaskDelay(ticks);
}
