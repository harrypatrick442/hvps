#pragma once
#include "esp_err.h"
#include "driver/gpio.h"
class IOInteruptHelper {
public:
	static esp_err_t setupPinEdgeInterupt(
		int pin,
		gpio_isr_t staticHandlerMethod,
		void* arg,
		bool risingEdge,
		bool fallingEdge,
		bool pullUpEnabled,
		bool pullDownEnabled
	);
	static esp_err_t removeHandlerAndDisableEdgeInterupt(int pin);
	static esp_err_t installISRHandlerIfNotAlready();
private:
};