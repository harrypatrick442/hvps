// InterruptTimer.hpp
#pragma once

#include <cstdint>
#include <mutex>
#include <memory>
#include "esp_err.h"
#include "driver/timer.h"
#include "esp_intr_alloc.h"
#include "Timing/HardwareTimerPool/HardwareTimerLease.hpp"

class InterruptTimer {
public:
    typedef bool  (*ISRHandler)(void*);

    InterruptTimer(uint32_t periodUs, int intrLevel = ESP_INTR_FLAG_LEVEL4, bool abortOnConfigureFailed = true);
    InterruptTimer(timer_group_t group, timer_idx_t idx, uint32_t periodUs, int intrLevel = ESP_INTR_FLAG_LEVEL4, bool abortOnConfigureFailed = true);
    ~InterruptTimer();

    esp_err_t configure(ISRHandler handler, void* context);
    esp_err_t start();
    esp_err_t stop();
	uint32_t getPeriodUs();
    esp_err_t updatePeriod(uint32_t newPeriodUs);
	esp_err_t release();

private:
	std::unique_ptr<HardwareTimerLease> _hardwareTimerLease;
    timer_group_t _group;
    timer_idx_t _idx;
    uint32_t _periodUs;
    int _intrLevel;
	bool _abortOnConfigureFailed;
    ISRHandler _handler;
    void* _context;
	bool _configured;
	std::mutex _mutex;
	esp_err_t _release();
    void clearInterrupt();
	void failed(const char* messageName, esp_err_t err);
	static bool IRAM_ATTR timerISRTrampoline(void* arg);
};