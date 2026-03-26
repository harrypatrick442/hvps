#pragma once
#include "driver/gpio.h"
#include <optional>
class IOHelper {
public:
	static void configureInputPin(int pin, std::optional<bool> pullDown);
	static bool getIsHigh(int pin);
	static void configureOutputPin(int pin, bool onElseOff);
	static void setOnElseOff(int pin, bool onElseOff);
};