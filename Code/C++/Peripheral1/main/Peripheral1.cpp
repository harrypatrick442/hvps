#include "Peripheral1.hpp"
#include "System/WatchdogFeeder.hpp"
#include "System/StayTheFuckAwake.hpp"
#include "System/Aborter.hpp"
#include "IO/Outputs.hpp"
#include "Ports/Port_FiberOpticChannel1.hpp"
#include "Enums/SubsystemIdentifiers.hpp"
#include "esp_log.h"
#include "Generated/Peripheral1Config.hpp"
#include "HVPSLEDDisplay.hpp"
#include "SystemStateIndicator.hpp"
#define WATCHDOG_TIMEOUT_MILLISECONDS 10000

extern "C" void app_main(void)
{
	Aborter::setToSafe(&Outputs::toSafe);
	Outputs::initialize();
	Outputs::toSafeReversible();
	esp_wifi_stop();
	esp_wifi_deinit();
    esp_log_set_vprintf(vprintf);
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    StayTheFuckAwake::initialize();
    WatchdogFeeder::initialize(WATCHDOG_TIMEOUT_MILLISECONDS);
	HVPSLEDDisplay& hVPSLEDDisplay 
		= HVPSLEDDisplay::initialize(Config1);
	SystemStateIndicator& systemStateIndicator 
		= SystemStateIndicator::initialize(hVPSLEDDisplay);
    Port_FiberOpticChannel1& port_FiberOpticChannel1 
		= Port_FiberOpticChannel1::initialize(
			SubsystemIdentifiers::Peripheral1, systemStateIndicator);
	vTaskDelete(NULL); // Delete the current task
}
