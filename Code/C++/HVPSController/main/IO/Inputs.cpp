#include "./Inputs.hpp"
#include "Logging/Log.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "PinDefinitions.hpp"
#include "ADCChannels.hpp"
#include "ADC/ADC.hpp"
#include "System/SafeAbort.hpp"

const char* Inputs::TAG = "Inputs";
bool Inputs::_initialized = false;

void Inputs::initialize() {
	if (_initialized) {
		LOG_ERROR("Already Initialized!");
		return;
	}
	_initialized = true;

    configureInputPin(PinDefinitions::OUTPUT_VOLTAGE_FEEDBACK_THRESHOLD_REACHED);
    configureInputPin(PinDefinitions::FIRST_STAGE_VOLTAGE_FEEDBACK_THRESHOLD_REACHED);
    configureInputPin(PinDefinitions::OUTPUT_CURRENT_FEEDBACK_THRESHOLD_REACHED);
		
}
#include <optional>
#include "driver/gpio.h"

void Inputs::configureInputPin(int pin, std::optional<bool> pullDown) {
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << pin);

    if (pullDown.has_value()) {
        if (*pullDown) {
            io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
            io_conf.pull_up_en   = GPIO_PULLUP_DISABLE;
        } else {
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            io_conf.pull_up_en   = GPIO_PULLUP_ENABLE;
        }
    } else {
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        io_conf.pull_up_en   = GPIO_PULLUP_DISABLE;
    }

    gpio_config(&io_conf);
}
bool Inputs::getOutputVoltageFeedbackThresholdReached(){
	checkInitialized();
	return gpio_get_level(
		(gpio_num_t)PinDefinitions::OUTPUT_VOLTAGE_FEEDBACK_THRESHOLD_REACHED
		)==1;
}
bool Inputs::getFirstStageVoltageFeedbackThresholdReached(){
	checkInitialized();
	return gpio_get_level(
		(gpio_num_t)PinDefinitions::FIRST_STAGE_VOLTAGE_FEEDBACK_THRESHOLD_REACHED
		)==1;
}
bool Inputs::getOutputCurrentFeedbackThresholdReached(){
	checkInitialized();
	return gpio_get_level(
		(gpio_num_t)PinDefinitions::OUTPUT_CURRENT_FEEDBACK_THRESHOLD_REACHED
		)==1;
}
void Inputs::checkInitialized(){
	if(!Inputs::_initialized){
        SAFE_ABORT("Not Initialized!");
	}
}

void Inputs::useADCReferenceVoltageChannel(const std::function<void(IADCSession&&)>& fn){
    ADC::use(ADCChannels::REFERENCE_VOLTAGE, fn);
}
void Inputs::useADCSnubberVoltageFeedbackChannel(const std::function<void(IADCSession&&)>& fn){
    ADC::use(ADCChannels::SNUBBER_VOLTAGE_FEEDBACK, fn);
}
void Inputs::useADCPrimaryCurrentFeedbackChannel(const std::function<void(IADCSession&&)>& fn){
    ADC::use(ADCChannels::PRIMARY_CURRENT_FEEDBACK, fn);
}
void Inputs::useADCPowerSupplyVoltageFeedbackChannel(const std::function<void(IADCSession&&)>& fn){
    ADC::use(ADCChannels::POWER_SUPPLY_VOLTAGE_FEEDBACK, fn);
}
