#include "./Outputs.hpp"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "Logging/Log.hpp"
#include "PinDefinitions.hpp"
#include "soc/gpio_struct.h"
const char* Outputs::TAG = "Outputs";
static portMUX_TYPE s_outputsMux = portMUX_INITIALIZER_UNLOCKED;
bool Outputs::s_initialized = false;
bool Outputs::s_safe = false;
void Outputs::initialize(){
	
    //portENTER_CRITICAL(&s_outputsMux);
    if (s_initialized) {
        //portEXIT_CRITICAL(&s_outputsMux);
        LOG_ERROR("Already Initialized!");
        return;
    }
	configureOutputPin(PinDefinitions::SOFT_START_RESISTOR_BYPASS, false);
    s_initialized = true;
    //portEXIT_CRITICAL(&s_outputsMux);
}
void Outputs::configureOutputPin(int pin, bool onElseOff){
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    esp_err_t err = gpio_config(&io_conf);
    if (err != ESP_OK) {
        LOG_ERROR("gpio_config failed for pin %d with err %d", pin, err);
    }
    gpio_set_level((gpio_num_t)pin, onElseOff ? 1 : 0);
}
void Outputs::toSafe(){
    portENTER_CRITICAL(&s_outputsMux);
    if (!s_safe) {
        s_safe = true;                    // irreversible by policy
        if (s_initialized) {
        }
    }
    portEXIT_CRITICAL(&s_outputsMux);
}
void Outputs::toSafeReversible(){
    portENTER_CRITICAL(&s_outputsMux);
	_setSoftStartResistorBypassOnOff(false);
    portEXIT_CRITICAL(&s_outputsMux);
}
void Outputs::setSoftStartResistorBypassOnOff(bool onElseOff){
    portENTER_CRITICAL(&s_outputsMux);
    if (!s_safe && s_initialized) {
		_setSoftStartResistorBypassOnOff(onElseOff);
    }
    portEXIT_CRITICAL(&s_outputsMux);
}
void Outputs::_setSoftStartResistorBypassOnOff(bool onElseOff){
	gpio_set_level((gpio_num_t)PinDefinitions::SOFT_START_RESISTOR_BYPASS, onElseOff?1:0);
}