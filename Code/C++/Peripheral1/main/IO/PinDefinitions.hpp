#pragma once
#include "driver/gpio.h"
class PinDefinitions{
public:
	static constexpr int TX = GPIO_NUM_14;//GPIO_NUM_27;
	static constexpr int RX = GPIO_NUM_13;//GPIO_NUM_14;
	static constexpr int LEDS_D_OUT = GPIO_NUM_33;
	static constexpr int DRIVE_SIGNAL = GPIO_NUM_27;
};