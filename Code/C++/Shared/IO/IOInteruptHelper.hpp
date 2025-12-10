#pragma once
class IOInteruptHelper {
public:
	static esp_err_t IOInteruptHelper::setupPinEdgeInterupt(
		int pin,
		gpio_isr_t staticHandlerMethod,
		void* arg,
		bool risingEdge,
		bool fallingEdge,
		bool pullUpEnabled,
		bool pullDownEnabled
	);
private:
	static esp_err_t installISRHandlerIfNotAlready();
};