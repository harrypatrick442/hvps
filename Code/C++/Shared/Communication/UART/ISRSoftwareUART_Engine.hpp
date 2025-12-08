#pragma once
#include "esp_timer.h"
#include "esp_attr.h"

class ISRSoftwareUART;

class ISRSoftwareUART_Engine
{
public:
    static constexpr int MAX_UARTS = 8;
    static constexpr int OVERSAMPLE = 8;

    static void init(int baud);
    static void registerInstance(ISRSoftwareUART* uart);

private:
    friend class ISRSoftwareUART;

    static void IRAM_ATTR onTimerISR(void* arg);

    static inline bool s_initialized = false;
    static inline int s_uartCount = 0;
    static inline ISRSoftwareUART* s_instances[MAX_UARTS] = {};
    static inline esp_timer_handle_t s_timer = nullptr;

    static inline int s_baud = 0;
    static inline int s_bitPeriodUs = 0;
    static inline int s_tickPeriodUs = 0;
};
