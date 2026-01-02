#include "DAC.hpp"
#include "esp_log.h"
#include "Macros/GetFileName.hpp"
const char* DAC::getTag() {return GET_FILE_NAME;}

DAC::DAC(bool enableChannel0, bool enableChannel1) noexcept :
_ch0(nullptr),
_ch1(nullptr) {
    if (enableChannel0) {
        dac_oneshot_config_t cfg = {
            .chan_id = DAC_CHAN_0// GPIO25
        };
		esp_err_t err = dac_oneshot_new_channel(&cfg, &_ch0);
        if (err == ESP_OK) {
			LOG_INFO("DAC_CHAN_0  (GPIO25) enabled");
        }
		else{
            SAFE_ABORT("Failed to init DAC_CHAN_0 with error: %s", esp_err_to_name(err));
		}
    }
    if (enableChannel1) {
        dac_oneshot_config_t cfg = {
            .chan_id = DAC_CHAN_1  // GPIO26
        };
		esp_err_t err = dac_oneshot_new_channel(&cfg, &_ch1);
        if (err == ESP_OK) {
			LOG_INFO("DAC_CHAN_1 (GPIO26) enabled");
        }
		else{
            SAFE_ABORT("Failed to init DAC_CHAN_1 with error: %s", esp_err_to_name(err));
		}
    }
}

DAC::~DAC() = default;
void DAC::setChannel0Voltage(float voltage, float vMax) {
	uint8_t value = voltageToValue(voltage, vMax);
	setChannel0Value(value);
}
void DAC::setChannel1Voltage(float voltage, float vMax) {
	uint8_t value = voltageToValue(voltage, vMax);
	setChannel1Value(value);
}
void DAC::setChannel0Value(uint8_t value) {
	if(_ch0==nullptr){
		SAFE_ABORT("DAC_CHAN_0 not initialized");
		return;
	}
    dac_oneshot_output_voltage(_ch0, value);
}
void DAC::setChannel1Value(uint8_t value) {
	if(_ch1==nullptr){
		SAFE_ABORT("DAC_CHAN_1 not initialized");
		return;
	}
    dac_oneshot_output_voltage(_ch1, value);
}
uint8_t DAC::voltageToValue(float voltage, float Vmax) {
    if (voltage < 0.0f) return 0;
    if (voltage > Vmax) return 255;
	float v = (voltage* 255.0f) / Vmax;
    return static_cast<uint8_t>(v);
}
