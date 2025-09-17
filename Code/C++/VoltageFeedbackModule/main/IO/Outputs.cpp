#include "./Outputs.hpp"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "../Logging/Log.hpp"

#define THRESHOLD_REACHED_FIBER_OPTIC_PIN 21
const char* Outputs::TAG = "Outputs";
bool Outputs::_initialized = false;
void Outputs::initialize(){
	if(Outputs::_initialized){
        Log::Error(TAG, "Already Initialized!");
		return;
	}
	Outputs::_initialized = true;
    gpio_set_direction((gpio_num_t)THRESHOLD_REACHED_FIBER_OPTIC_PIN, 
		GPIO_MODE_OUTPUT);
}
void Outputs::toSafe(){
	
}
void Outputs::setThresholdReachedFiberOpticOnOff(bool onElseOff){
	if(!Outputs::_initialized){
        Log::Error(TAG, "Not Initialized!");
		return;
	}
	//0 IS USED FOR THRESHOLD REACHED. USING 0 MEANS IN EVENT OF POWER FAILURE DRIVE STOPS.
    gpio_set_level((gpio_num_t)THRESHOLD_REACHED_FIBER_OPTIC_PIN, onElseOff?1:0); // Set GPIO21 HIGH (on)
}