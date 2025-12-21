#include "./Inputs.hpp"
#include "Logging/Log.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "PinDefinitions.hpp"
#include "System/SafeAbort.hpp"
#include "driver/gpio.h"
#include <optional>

const char* Inputs::TAG = "Inputs";
bool Inputs::_initialized = false;

void Inputs::initialize() {
	if (_initialized) {
		LOG_ERROR("Already Initialized!");
		return;
	}
	_initialized = true;

    configureInputPin(PinDefinitions::PinDefinitions::DRIVE_SIGNAL);
		
}

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
bool Inputs::getDriveSignal(){
	checkInitialized();
	return gpio_get_level(
		(gpio_num_t)PinDefinitions::DRIVE_SIGNAL
		)==1;
}
void Inputs::checkInitialized(){
	if(!Inputs::_initialized){
        SAFE_ABORT("Not Initialized!");
	}
}
