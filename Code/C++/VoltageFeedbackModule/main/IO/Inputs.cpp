#include "./Inputs.hpp"
#include "../Logging/Log.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

const char* Inputs::TAG = "Inputs";
bool Inputs::_initialized = false;

void Inputs::initialize() {
	if (_initialized) {
		Log::Error(TAG, "Already Initialized!");
		return;
	}
	_initialized = true;

	gpio_config_t io_conf = {};
	io_conf.intr_type = GPIO_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pin_bit_mask = (1ULL << RECEIVER_INPUT_PIN);
	io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	io_conf.pull_up_en = GPIO_PULLUP_DISABLE;

	gpio_config(&io_conf);
	Log::Info(TAG, "Receiver input on GPIO%d initialized", RECEIVER_INPUT_PIN);
}

int Inputs::readReceiver() {
	if (!_initialized) {
		Log::Error(TAG, "Not Initialized!");
		return -1;
	}
	return gpio_get_level((gpio_num_t)RECEIVER_INPUT_PIN);
}
