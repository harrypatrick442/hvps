#include "./Outputs.hpp"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "Logging/Log.hpp"

#define THRESHOLD_REACHED_FIBER_OPTIC_PIN 21
bool Outputs::_initialized = false;
void Outputs::initialize(){
	if(Outputs::_initialized){
        LOG_ERROR("Already Initialized!");
		return;
	}
	Outputs::_initialized = true;
    gpio_set_direction((gpio_num_t)THRESHOLD_REACHED_FIBER_OPTIC_PIN, 
		GPIO_MODE_OUTPUT);
}
void Outputs::toSafeReversible(){
	
}
void Outputs::toSafe(){
	
}
void Outputs::setThresholdReached(bool isReached){
	if(!Outputs::_initialized){
        LOG_ERROR("Not Initialized!");
		return;
	}
	//0 IS USED FOR THRESHOLD REACHED. USING 0 MEANS IN EVENT OF POWER FAILURE DRIVE STOPS.
    gpio_set_level((gpio_num_t)THRESHOLD_REACHED_FIBER_OPTIC_PIN, isReached?0:1); // Set GPIO21 HIGH (on)
}