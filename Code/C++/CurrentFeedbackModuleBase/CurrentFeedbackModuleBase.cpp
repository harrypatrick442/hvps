#include "CurrentFeedbackModuleBase.hpp"
#include "ADC/ADC.hpp"
#include "IO/Outputs.hpp"
#include "IO/Inputs.hpp"
#include "Ports/Port_FiberOpticChannel1.hpp"
#include "Storage/Flash.hpp"
#include "System/WatchdogFeeder.hpp"
#include "System/StayTheFuckAwake.hpp"
#include "System/SafeAbort.hpp"
#include "CurrentAndPowerThresholdMonitor.hpp"
#include "Generated/CurrentAndPowerFeedbackModuleConfiguration.hpp"
#define WATCHDOG_TIMEOUT_MILLISECONDS 10000

void CurrentFeedbackModuleBase::main(const Configuration& config1, const Configuration& config2)
{
	Aborter::setToSafe(&Outputs::toSafe);
	Outputs::initialize();
	Outputs::toSafeReversible();
	ADC::initialize();
	Inputs::initialize();
	
	esp_wifi_stop();
	esp_wifi_deinit();
    esp_log_set_vprintf(vprintf);
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    StayTheFuckAwake::initialize();
    WatchdogFeeder::initialize(WATCHDOG_TIMEOUT_MILLISECONDS);
	
	Flash::initialize();
	Inputs::selectADCVoltageDividerInputAsChannel();
	CurrentAndPowerThresholdMonitor::initialize(config1, config2);
    Port_FiberOpticChannel1& port_FiberOpticChannel1 = Port_FiberOpticChannel1::initialize();
	vTaskDelete(NULL); // Delete the current task
}
