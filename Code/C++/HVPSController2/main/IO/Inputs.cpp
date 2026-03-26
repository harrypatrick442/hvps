#include "./Inputs.hpp"
#include "Logging/Log.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "PinDefinitions.hpp"
#include "ADCChannels.hpp"
#include "ADC/ADCOneShot.hpp"
#include "System/SafeAbort.hpp"

const char* Inputs::TAG = "Inputs";
bool Inputs::_initialized = false;

void Inputs::initialize() {
	if (_initialized) {
		LOG_ERROR("Already Initialized!");
		return;
	}
	_initialized = true;
		
}
#include <optional>
#include "driver/gpio.h"

void Inputs::checkInitialized(){
	if(!Inputs::_initialized){
        SAFE_ABORT("Not Initialized!");
	}
}
void Inputs::useADCPowerSupplyVoltageFeedbackChannel(const std::function<void(IADCSession&&)>& fn){
    ADCOneShot::use(ADCChannels::POWER_SUPPLY_VOLTAGE_FEEDBACK, fn);
}