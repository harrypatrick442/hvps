#include "System/WatchdogFeeder.hpp"
#include "System/StayTheFuckAwake.hpp"
#include "Timing/Delay.hpp"
#include "esp_err.h"
#include "esp_log.h"
#include "Logging/Log.hpp"
#include "Timing/Delay.hpp"
#include "IO/PinDefinitions.hpp"
#include "Components/TemperatureSensors/DHT22.hpp"
#define WATCHDOG_TIMEOUT_MILLISECONDS 10000

extern "C" void app_main(void)
{
	esp_wifi_stop();
	esp_wifi_deinit();
    esp_log_set_vprintf(vprintf);
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    StayTheFuckAwake::initialize();
    WatchdogFeeder::initialize(WATCHDOG_TIMEOUT_MILLISECONDS);
	DHT22 dht22(PinDefinitions::TEMPERATURE_SENSOR_1);
	float temperature;
	float humidity;
	while(true){
		Delay::ms(1000);
		esp_err_t err = dht22.getTemperatureAndHumidity(temperature, humidity);
		if (err == ESP_OK) {
			LOG_INFO("Temperature: %.2f °C, Humidity: %.2f %%", temperature, humidity);
		} else {
			LOG_ERROR("DHT22 read failed: %s", esp_err_to_name(err));
		}		
	}
	vTaskDelete(NULL); // Delete the current task
}
