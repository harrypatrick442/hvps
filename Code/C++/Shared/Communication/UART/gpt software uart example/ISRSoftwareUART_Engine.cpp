#include "ISRSoftwareUART_Engine.hpp"
#include "ISRSoftwareUART.hpp"
#include "esp_timer.h"

void ISRSoftwareUART_Engine::init(int baud)
{
    if (s_initialized) return;

    s_baud = baud;
    s_bitPeriodUs = 1000000 / baud;
    s_tickPeriodUs = s_bitPeriodUs / OVERSAMPLE;

    esp_timer_create_args_t args = {};
    args.callback = &ISRSoftwareUART_Engine::onTimerISR;
    args.arg = nullptr;
    args.dispatch_method = ESP_TIMER_ISR;
    args.name = "softuart";

    esp_timer_create(&args, &s_timer);
    esp_timer_start_periodic(s_timer, s_tickPeriodUs);

    // Pin ISR to Core 0
    esp_timer_set_cpu(s_timer, 0);

    s_initialized = true;
}

void ISRSoftwareUART_Engine::registerInstance(ISRSoftwareUART* uart)
{
    if (s_uartCount < MAX_UARTS)
        s_instances[s_uartCount++] = uart;
}

void IRAM_ATTR ISRSoftwareUART_Engine::onTimerISR(void* arg)
{
    for (int i = 0; i < s_uartCount; i++)
    {
        ISRSoftwareUART* u = s_instances[i];
        if (!u) continue;

        u->handleRxISR();
        u->handleTxISR();
    }
}
