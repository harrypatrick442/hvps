#include "./Outputs.hpp"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "../Logging/Log.hpp"
#include "PinDefinitions.hpp"

const char* Outputs::TAG = "Outputs";
static portMUX_TYPE s_outputsMux = portMUX_INITIALIZER_UNLOCKED;
bool Outputs::_initialized = false;
bool Outputs::_safe = false;
void Outputs::initialize(){
	
	
    portENTER_CRITICAL(&s_outputsMux);
    if (s_initialized) {
        portEXIT_CRITICAL(&s_outputsMux);
        Log::Error(TAG, "Already Initialized!");
        return;
    }
	configureOutputPin(PinDefinitions::MOSFET_DRIVE, false);
    s_initialized = true;
    portEXIT_CRITICAL(&s_outputsMux);
}
void Outputs::configureOutputPin(int pin, bool onElseOff){
    gpio_set_direction(
		(gpio_num_t)pin, 
		GPIO_MODE_OUTPUT);
	gpio_set_level((gpio_num_t)pin, onElseOff?1:0);
}
void Outputs::toSafe(){
    portENTER_CRITICAL(&s_outputsMux);
    if (!s_safe) {
        s_safe = true;                    // irreversible by policy
        if (s_initialized) {
			_setMOSFETOnOff(false);
        }
    }
    portEXIT_CRITICAL(&s_outputsMux);
}
void Outputs::setMOSFETOnOff(bool onElseOff){
    portENTER_CRITICAL(&s_outputsMux);
    if (!s_safe && s_initialized) {
		_setMOSFETOnOff(onElseOff);
    }
    portEXIT_CRITICAL(&s_outputsMux);
}
void Outputs::_setMOSFETOnOff(bool onElseOff){
	gpio_set_level((gpio_num_t)PinDefinitions::MOSFET_DRIVE, onElseOff?1:0);
}