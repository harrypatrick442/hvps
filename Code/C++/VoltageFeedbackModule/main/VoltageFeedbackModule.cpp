#include "esp_log.h"
#include <array>
#include "freertos/FreeRTOSConfig.h"
#include "./Logging/Log.hpp"
#include "./System/WatchdogFeeder.hpp"
#include "./System/StayTheFuckAwake.hpp"
#include "./Timing/Delay.hpp"
#include "./Generated/Messages/PingMessage.hpp"
#include "./IO/Outputs.hpp"
#include "./IO/Inputs.hpp"
#include "./IO/AnalogueInputs.hpp"
#include "./Storage/Flash.hpp"
#include "ADC/MonitorVoltageThresholdHandle.hpp"
#include "Ports/Port_FiberOpticChannel1.hpp"
#include "PriorityLooper.hpp"
#include <iostream>
#include <array>
#include <sstream>
#include <iomanip>
#ifndef portTICK_PERIOD_MS
#define portTICK_PERIOD_MS (1000 / configTICK_RATE_HZ)
#endif
#define WATCHDOG_TIMEOUT_MILLISECONDS 10000
#include "esp_timer.h"
static const char *TAG = "VoltageFeedbackModule";

extern "C" void app_main(void)
{
	Outputs::initialize();
	Outputs::toSafe();
	Delay::ms(1000);
	AnalogueInputs::initialize();
	Inputs::initialize();
	esp_wifi_stop();
	esp_wifi_deinit();
    esp_log_set_vprintf(vprintf);
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    ESP_LOGI(TAG, "Starting Voltage Feedback Module...");
    StayTheFuckAwake::initialize();
    WatchdogFeeder::initialize(WATCHDOG_TIMEOUT_MILLISECONDS);
	Flash::initialize();
	AnalogueInputs::selectVoltageDividerInputAsChannel();
	MonitorVoltageThresholdHandle* monitorVoltageThresholdHandle = 
		AnalogueInputs::monitorVoltageThresholdWithNewPriorityTask(0);
	State state = State::initialize(monitorVoltageThresholdHandle);
    Port_FiberOpticChannel1& port_FiberOpticChannel1 = Port_FiberOpticChannel1::initialize(&state);

	bool isHigh = true;
	while(true){
		Delay::ms(10000);
		if(port_FiberOpticChannel1.setVoltageThreshold(isHigh?3.0:0.0)){
			Log::Info(TAG, "Set voltage successfully");
		}
		isHigh = ! isHigh;
	}
	vTaskDelete(NULL); // Delete the current task
}
