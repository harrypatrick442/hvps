#include "esp_log.h"
#include <array>
#include "freertos/FreeRTOSConfig.h"
#include "./Communication/Bluetooth/Bluetooth_BR_EDR.hpp"
#include "./Logging/Log.hpp"
#include "./System/WatchdogFeeder.hpp"
#include "./System/StayTheFuckAwake.hpp"
#include "./IO/Outputs.hpp"
#include "./IO/Inputs.hpp"
#include "./IO/AnalogueInputs.hpp"
#include "./IO/SoftStartHandler.hpp"
#include "./Timing/Delay.hpp"
#include "./Communication/I2C/I2C.hpp"
#include "./Communication/I2C/I2CConfiguration.hpp"
#include "Ports/Port_FirstStageVoltageFeedback.hpp"
#include "Ports/Port_OutputVoltageFeedback.hpp"
#include "./Ports/Port_ControllingMachine.hpp"
#include "./Broadcasting/LiveDataBroadcaster.hpp"
#include "./Core/Checksums/Crc32.hpp"
#include "./generated/HVPSConfig.hpp"
#include <iostream>
#include <array>
#include <sstream>
#include <iomanip>
#ifndef portTICK_PERIOD_MS
#define portTICK_PERIOD_MS (1000 / configTICK_RATE_HZ)
#endif

#define WATCHDOG_TIMEOUT_MILLISECONDS 10000

static const char *TAG = "HVPS";

extern "C" void app_main(void)
{
	Outputs::initialize();
	Outputs::toSafeReversible();
	validateConfiguration();
	SoftStartHandler::doSoftStart();
	AnalogueInputs::initialize();
	Inputs::initialize();
	esp_wifi_stop();
	esp_wifi_deinit();
    esp_log_set_vprintf(vprintf);
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    ESP_LOGI(TAG, "Starting HVPSController...");
    StayTheFuckAwake::initialize();
	
    // Initialize the I2C bus
	I2CConfiguration i2cConfiguration;//Default
    I2C::initialize(i2cConfiguration);
    //I2C& i2c = I2C::getInstance();
    WatchdogFeeder
        ::initialize(WATCHDOG_TIMEOUT_MILLISECONDS);
		
	Port_FirstStageVoltageFeedback& port_FirstStageVoltageFeedback = Port_FirstStageVoltageFeedback::initialize();
	Port_OutputVoltageFeedback& port_OutputVoltageFeedback = Port_OutputVoltageFeedback::initialize();
    Bluetooth::initialize(
        "HVPS", 
        "HVPSControllerServer"
     );
    Bluetooth& bluetooth = Bluetooth::getInstance();
	LiveDataCache& liveDataCache = LiveDataCache::initialize(
		port_FirstStageVoltageFeedback,
		port_OutputVoltageFeedback
	);
	HighSpeedCore& highSpeedCore = HighSpeedCore::initialize(
		port_FirstStageVoltageFeedback,
		port_OutputVoltageFeedback,
		liveDataCache
	);
	Port_ControllingMachine& portControllingMachine 
		= Port_ControllingMachine::initialize(bluetooth, highSpeedCore);
							 
	LiveDataBroadcaster::initialize(liveDataCache, portControllingMachine);
	vTaskDelete(NULL); // Delete the current task
}  
