#include "TimerInterruptHelper.hpp"

TimerInterruptHandle TimerInterruptHelper::usingTimerInterrupt(
    uint64_t periodUs,
    esp_timer_cb_t isr,
    void* argumentForIsr,
	const char* name)
{
    esp_timer_handle_t timer = nullptr;

    esp_timer_create_args_t args = {
        .callback = isr,
        .arg = argumentForIsr,
        .dispatch_method = ESP_TIMER_TASK,
        .name = name
    };

    if (esp_timer_create(&args, &timer) != ESP_OK) {
        return TimerInterruptHandle(nullptr);
    }

    esp_timer_start_periodic(timer, periodUs);

    return TimerInterruptHandle(timer);
}
