#include "IOHelper.hpp"
#include "Logging/Log.hpp"
void IOHelper::configureInputPin(int pin, std::optional<bool> pullDown) {
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

    esp_err_t err = gpio_config(&io_conf);
    if (err != ESP_OK) {
        LOG_ERROR("gpio_config failed for pin %d with err %d", pin, esp_err_to_name(err));
    }
}
void IOHelper::configureOutputPin(int pin, bool onElseOff){
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    esp_err_t err = gpio_config(&io_conf);
    if (err != ESP_OK) {
        LOG_ERROR("gpio_config failed for pin %d with err %d", pin, esp_err_to_name(err));
    }
    gpio_set_level((gpio_num_t)pin, onElseOff ? 1 : 0);
}
bool IOHelper::getIsHigh(int pin){
	bool value =  gpio_get_level(
		(gpio_num_t)pin
		)==1;
	return value;
}

void IOHelper::setOnElseOff(int pin, bool onElseOff){
	gpio_set_level((gpio_num_t)pin, onElseOff?1:0);
}