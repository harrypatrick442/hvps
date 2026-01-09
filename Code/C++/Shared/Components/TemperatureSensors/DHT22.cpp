#pragma once
#include "DHT22.hpp"
#include "../core/dht.h"

DHT22::DHT22(int pin):
_pin((gpio_num_t) pin){
	
}
esp_err_t DHT22::getTemperatureAndHumidity(float& temperature, float& humidity) {
	return  dht_read_float_data(DHT_TYPE_AM2301, _pin,
        &humidity, &temperature);
}