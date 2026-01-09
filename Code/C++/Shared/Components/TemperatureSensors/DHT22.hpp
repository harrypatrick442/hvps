#pragma once
#include "../core/dht.h"
#include <driver/gpio.h>
#include <esp_err.h>
class DHT22{
private:
	gpio_num_t _pin;
public:
	DHT22(int pin);
	esp_err_t getTemperatureAndHumidity(float& temperature, float& humidity);
};