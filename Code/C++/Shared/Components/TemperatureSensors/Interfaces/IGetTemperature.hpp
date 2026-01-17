#pragma once
#include <cstdint>
#include "esp_err.h"
class IGetTemperature{
	public: virtual esp_err_t getTemperature(float& temperature)=0;
};