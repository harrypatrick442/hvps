#include "./Inputs.hpp"
#include "../Logging/Log.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "PinDefinitions.hpp"
#include "../System/Aborter.hpp"

const char* Inputs::TAG = "Inputs";
bool Inputs::_initialized = false;

void Inputs::initialize() {
	if (_initialized) {
		Log::Error(TAG, "Already Initialized!");
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
		)==0;
}
bool Inputs::getFirstStageVoltageFeedbackThresholdReached(){
	checkInitialized();
	return gpio_get_level(
		(gpio_num_t)PinDefinitions::FIRST_STAGE_VOLTAGE_FEEDBACK_THRESHOLD_REACHED
		)==0;
}
bool Inputs::getOutputCurrentFeedbackThresholdReached(){
	checkInitialized();
	return gpio_get_level(
		(gpio_num_t)PinDefinitions::OUTPUT_CURRENT_FEEDBACK_THRESHOLD_REACHED
		)==0;
}
void Inputs::checkInitialized(){
	if(!Inputs::_initialized){
        Aborter::safeAbort(TAG, "Not Initialized!");
	}
}