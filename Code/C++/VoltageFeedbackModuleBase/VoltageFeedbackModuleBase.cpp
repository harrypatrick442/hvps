#include "VoltageFeedbackModuleBase.hpp"
#include "System/WatchdogFeeder.hpp"
#include "System/StayTheFuckAwake.hpp"
#include "System/Aborter.hpp"
#include "IO/Outputs.hpp"
#include "IO/Inputs.hpp"
#include "IO/ADCChannels.hpp"
#include "Storage/Flash.hpp"
#include "ADC/ADC.hpp"
#include "Ports/Port_FiberOpticChannel1.hpp"
#include "ThresholdMonitor.hpp"
#include "esp_log.h"
#define WATCHDOG_TIMEOUT_MILLISECONDS 10000

void VoltageFeedbackModuleBase::main(const VoltageFeedbackModuleConfiguration& config1,
 const VoltageFeedbackModuleConfiguration& config2, std::function<void()> validateConfig)
{
	Aborter::setToSafe(&Outputs::toSafe);
	Outputs::initialize();
	Outputs::toSafeReversible();
	validateConfig();
	ADC::initialize();
	Inputs::initialize();
	esp_wifi_stop();
	esp_wifi_deinit();
    esp_log_set_vprintf(vprintf);
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    StayTheFuckAwake::initialize();
    WatchdogFeeder::initialize(WATCHDOG_TIMEOUT_MILLISECONDS);
	Flash::initialize();
	ThresholdMonitor& thresholdMonitor 
		= ThresholdMonitor::initialize(
			ADCChannels::VOLTAGE_DIVIDER_INPUT,
			config1,
			config2
		);
	Port_FiberOpticChannel1::initialize(thresholdMonitor);
/*
	bool isHigh = true;
	while(true){
		Delay::ms(10000);
		if(port_FiberOpticChannel1.setVoltageThreshold(isHigh?3.0:0.0)){
			Log::Info(TAG, "Set voltage successfully");
		}
		isHigh = ! isHigh;
	}*/
	vTaskDelete(NULL); // Delete the current task
}
