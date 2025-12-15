#include "DAC.hpp"
#include "esp_log.h"

DAC::DAC(bool enableChannel1, bool enableChannel2) noexcept {
    // Enable DAC channel 1 (GPIO25)
	if(enableChannel1){
		dac_output_enable(DAC_CHANNEL_1);
		Log::Info(TAG, "DAC channel 1 (GPIO25) enabled");
	}
	if(enableChannel2){
		dac_output_enable(DAC_CHANNEL_2);
		Log::Info(TAG, "DAC channel 2 (GPIO26) enabled");
	}
}

DAC::~DAC() = default;
void DAC::setChannel1Voltage(float voltage, float vMax) {
	setChannelVoltage(DAC_CHANNEL_1, voltage, vMax);
}
void DAC::setChannel2Voltage(float voltage, float vMax) {
	setChannelVoltage(DAC_CHANNEL_2, voltage, vMax);
}
void DAC::setChannelVoltage(dac_channel_t channel, float voltage, float vMax) {
	uint8_t value = voltageToValue(voltage, vMax);
	setChannelValue(channel, value);
}
void DAC::setChannel1Value(uint8_t value) {
	setChannelValue(DAC_CHANNEL_1, value);
}
void DAC::setChannel2Value(uint8_t value) {
	setChannelValue(DAC_CHANNEL_2, value);
}
void DAC::setChannelValue(dac_channel_t channel, uint8_t value) {
    if (value > 255) value = 255;
    dac_output_voltage(channel, value);
}
uint8_t DAC::voltageToValue(float voltage, float Vmax) {
    if (voltage < 0.0f) voltage = 0.0f;
    if (voltage > Vmax) voltage = Vmax;
    return static_cast<uint8_t>((voltage / Vmax) * 255.0f);
}
