// InterruptTimer.cpp
#include "InterruptTimer.hpp"
#include "esp_log.h"
#include "System/SafeAbort.hpp"
#include "Macros/GetFileName.hpp"
#include "Logging/Log.hpp"
#include "driver/timer.h"
#include "Timing/HardwareTimerPool/HardwareTimerPool.hpp"

InterruptTimer::InterruptTimer( 
uint32_t periodUs, int intrLevel, bool abortOnConfigureFailed)
    : 
	_hardwareTimerLease(HardwareTimerPool::acquire(GET_FILE_NAME)),
	_group(_hardwareTimerLease->getGroup()), _idx(_hardwareTimerLease->getIndex()),
	_periodUs(periodUs), _intrLevel(intrLevel), 
	_abortOnConfigureFailed(abortOnConfigureFailed),
	_handler(nullptr), _context(nullptr), _configured(false)
{
	
}
InterruptTimer::InterruptTimer(timer_group_t group, timer_idx_t idx, 
uint32_t periodUs, int intrLevel, bool abortOnConfigureFailed)
    : _hardwareTimerLease(nullptr), _group(group), _idx(idx), _periodUs(periodUs), _intrLevel(intrLevel), 
	_abortOnConfigureFailed(abortOnConfigureFailed), 
	_handler(nullptr), _context(nullptr), _configured(false)
{
	
}

InterruptTimer::~InterruptTimer() {
    release();
}
esp_err_t InterruptTimer::configure(ISRHandler handler, void* context) {
    std::lock_guard<std::mutex> lock(_mutex);
	if(_configured){
		LOG_WARN("Already configured");
		return ESP_ERR_INVALID_STATE;
	}
    _handler = handler;
    _context = context;

    timer_config_t config = { // or TIMER_SRC_CLK_XTAL
		.alarm_en = TIMER_ALARM_EN,
		.counter_en = TIMER_PAUSE,
		.intr_type = TIMER_INTR_LEVEL,
		.counter_dir = TIMER_COUNT_UP,
		.auto_reload = TIMER_AUTORELOAD_EN,
		.clk_src = TIMER_SRC_CLK_APB,
		.divider = 80
    };
    esp_err_t err = timer_init(_group, _idx, &config);
    if (err != ESP_OK) {
        failed("timer_init", err);
        _release();
        return err;
    }
	
	err = timer_set_counter_value(_group, _idx, 0);
	if (err != ESP_OK) {
		failed("timer_set_counter_value", err);
		_release();
		return err;
	}

    err = timer_set_alarm_value(_group, _idx, _periodUs);
    if (err != ESP_OK) {
        failed("timer_set_alarm_value", err);
        _release();
        return err;
    }

    err = timer_enable_intr(_group, _idx);
    if (err != ESP_OK) {
        failed("timer_enable_intr", err);
        _release();
        return err;
    }
    if ((_intrLevel != ESP_INTR_FLAG_LEVEL1) &&
        (_intrLevel != ESP_INTR_FLAG_LEVEL2) &&
        (_intrLevel != ESP_INTR_FLAG_LEVEL3)
	)
    {
        SAFE_ABORT("Invalid interrupt level flag with value %d", _intrLevel);
        _release();
        return err;
    }
    err = timer_isr_callback_add(
		_group,
		_idx,
		timerISRTrampoline,
		this,
		ESP_INTR_FLAG_IRAM | _intrLevel|ESP_INTR_FLAG_SHARED
	);

    if (err != ESP_OK) {
        failed("timer_isr_callback_add", err);
        _release();
        return err;
    }
	_configured = true;
    return ESP_OK;
}
bool IRAM_ATTR InterruptTimer::timerISRTrampoline(void* arg)
{
    auto* self = static_cast<InterruptTimer*>(arg);
    bool shouldYield = false;
    if (self->_handler) {
        shouldYield = self->_handler(self->_context);
    }

    self->clearInterrupt();
    return shouldYield;
}

void InterruptTimer::failed(const char* methodName, esp_err_t err){
	if(!_abortOnConfigureFailed){
		SAFE_ABORT("%s failed %s", methodName, esp_err_to_name(err));
		return;
	}
	LOG_WARN("%s failed %s", methodName, esp_err_to_name(err));
}

esp_err_t InterruptTimer::release() {
    std::lock_guard<std::mutex> lock(_mutex);
	if(!_configured){
		return ESP_OK;
	}
	return _release();
}
esp_err_t InterruptTimer::_release() {
	if(!_configured){
		return ESP_OK;
	}esp_err_t errToReturn = ESP_OK;

	// (1) Pause timer first
	esp_err_t err = timer_pause(_group, _idx);
	if (errToReturn == ESP_OK) errToReturn = err;

	// (2) Disable interrupt at hardware level
	err = timer_disable_intr(_group, _idx);
	if (errToReturn == ESP_OK) errToReturn = err;

/* not required
	// (3) Free the ISR
	if (_handle) {
		err = esp_intr_free(_handle);
		if (errToReturn == ESP_OK) errToReturn = err;
		_handle = nullptr;
	}
*/
	// (4) Deinit timer instance
	err = timer_deinit(_group, _idx);
	if (errToReturn == ESP_OK) errToReturn = err;

	// (5) Clear internal state
	//_handler = nullptr;
	//_context = nullptr;
	_configured = false;

	return errToReturn;

}


esp_err_t InterruptTimer::start() {
    return timer_start(_group, _idx);
}

esp_err_t InterruptTimer::stop() {
    return timer_pause(_group, _idx);
}
uint32_t InterruptTimer::getPeriodUs(){
	return _periodUs;
}
esp_err_t InterruptTimer::updatePeriod(uint32_t newPeriodUs) {
    _periodUs = newPeriodUs;
    return timer_set_alarm_value(_group, _idx, _periodUs);
}

void InterruptTimer::clearInterrupt() {
	timer_group_clr_intr_status_in_isr(_group, _idx);
}
