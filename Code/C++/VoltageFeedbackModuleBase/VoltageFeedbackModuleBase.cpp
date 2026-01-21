#include "VoltageFeedbackModuleBase.hpp"
#include "System/StayTheFuckAwake.hpp"
#include "System/SafeAbort.hpp"
#include "IO/Outputs.hpp"
#include "IO/ADCChannels.hpp"
#include "Storage/Flash.hpp"
#include "ADC/ADC.hpp"
#include "Ports/Port_FiberOpticChannel1.hpp"
#include "Broadcasting/VoltageBroadcaster.hpp"
#include "ThresholdMonitor.hpp"
#include "esp_log.h"

void VoltageFeedbackModuleBase::main(const VoltageFeedbackModuleConfiguration& config1,
 const VoltageFeedbackModuleConfiguration& config2, std::function<void()> validateConfig)
{
	Aborter::initialize(&Outputs::toSafe);
	Outputs::initialize();
	Outputs::toSafeReversible();
	validateConfig();
	ADC::initialize();
	esp_wifi_stop();
	esp_wifi_deinit();
    esp_log_set_vprintf(vprintf);
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    StayTheFuckAwake::disableSleepSources();
    StayTheFuckAwake::disablePowerManagement();
    StayTheFuckAwake::disableWiFiPowerSave();
    StayTheFuckAwake::disableWatchdog();
	Flash::initialize();
	ThresholdMonitor& thresholdMonitor 
		= ThresholdMonitor::initialize(
			ADCChannels::VOLTAGE_DIVIDER_INPUT,
			config1,
			config2
		);
	Port_FiberOpticChannel1& port = Port_FiberOpticChannel1::initialize(thresholdMonitor);
	VoltageBroadcaster::initialize(thresholdMonitor, port);
/*
	bool isHigh = true;
	while(true){
		Delay::ms(10000);
		if(port_FiberOpticChannel1.setVoltageThreshold(isHigh?3.0:0.0)){
			LOG_INFO("Set voltage successfully");
		}
		isHigh = ! isHigh;
	}*/
	vTaskDelete(NULL); // Delete the current task
}
