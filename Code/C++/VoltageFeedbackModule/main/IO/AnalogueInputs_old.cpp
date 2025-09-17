#include "./AnalogueInputs.hpp"
#include <stdio.h>
#include "driver/adc.h"
#include "esp_system.h"
#include "../Logging/Log.hpp"
#include "../Timing/Delay.hpp"
#include "esp_timer.h"
#include <cstdint> 
const char* AnalogueInputs::TAG = "AnalogueInputs";
bool AnalogueInputs::_initialized = false;
esp_adc_cal_characteristics_t* AnalogueInputs::_adc_chars = nullptr;
double AnalogueInputs::_correctionFactor = 1.0;
double AnalogueInputs::_voltageToRaw = 0.0;
std::optional<adc1_channel_t> AnalogueInputs::_currentChannel = std::nullopt;

void AnalogueInputs::initialize() {
    if (_initialized) {
        Log::Error(TAG, "Already Initialized!");
        return;
    }

    _initialized = true;
	
    _adc_chars = static_cast<esp_adc_cal_characteristics_t*>(
        calloc(1, sizeof(esp_adc_cal_characteristics_t))
    );

    if (_adc_chars == nullptr) {
        Log::Error(TAG, "Failed to allocate memory for ADC characteristics!");
        return;
    }

    // Configure all ADC channels used
    adc1_config_width(ADC_WIDTH);
    adc1_config_channel_atten(ADC_CHANNEL_VOLTAGE_DIVIDER_INPUT, ADC_ATTEN);

    // Characterize ADC
    esp_adc_cal_characterize(
        ADC_UNIT_1,
        ADC_ATTEN,
        ADC_WIDTH,
        ADC_CHARACTERIZATION_VREF,
        _adc_chars
    );

	
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S_MSB,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = BUF_COUNT,
		/*I choose only one buffer value because this means for single samples when i need the latest data
		quickly i can just sample and get it*/
        .dma_buf_len = BUF_LEN,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0
    };
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
	_correctionFactor = 1.0;
}
int AnalogueInputs::getTotalBuffersLength(){
	return BUF_COUNT* BUF_LEN;
}
void AnalogueInputs::setChannel(adc1_channel_t  channel){
	if(_currentChannel.has_value()){
		if(_currentChannel.value()==channel){
			return;
		}
		i2s_adc_disable(I2S_NUM_0);  // or whatever I2S_NUM you're using
	}
	_currentChannel = channel;
    i2s_set_adc_mode(ADC_UNIT_1, channel);
    // Configure ADC channel
    adc1_config_width(ADC_WIDTH_BIT_12);  // 12-bit width
    adc1_config_channel_atten(channel, ADC_ATTEN_DB_12);  // 0–3.3V

    i2s_adc_enable(I2S_NUM_0);
    i2s_zero_dma_buffer(I2S_NUM_0);
}
uint16_t AnalogueInputs::singleRawLatestSampleSelectedI2SChannel(){
    i2s_zero_dma_buffer(I2S_NUM_0);
	return singleRawSampleSelectedI2SChannel();
}
uint16_t AnalogueInputs::singleRawSampleSelectedI2SChannel() {
    //i2s_zero_dma_buffer(I2S_NUM_0);
    uint16_t sample = 0;
    size_t bytes_read = 0;
	uint16_t value = 0;
	while(value==0){
		/*ADC is not perfectly rail to rail and never returns 0.
		So if 0 that means buffer has not caught up yet.*/
		
		// Read 1 sample (2 bytes because of 16-bit width)
		esp_err_t err = i2s_read(I2S_NUM_0, &sample, sizeof(sample), &bytes_read, portMAX_DELAY);

		if (err != ESP_OK || bytes_read != sizeof(sample)) {
			Log::Error(TAG, "Failed to read I2S ADC sample");
			return 0;
		}
		// Mask to 12 bits (ESP32 ADC is 12-bit)
		value =  sample & 0x0FFF;
	}
	return value;
}
uint16_t AnalogueInputs::averagedRawSampleSelectedI2SChannel() {
	return averagedRawSampleSelectedI2SChannel(getTotalBuffersLength());
}
uint16_t AnalogueInputs::averagedRawSampleSelectedI2SChannel(int nSamples) {
    if (nSamples <= 0) return 0;

    const int sampleSize = sizeof(uint16_t);
    uint16_t rawSample = 0;
    size_t bytesRead = 0;
    uint32_t sum = 0;
	uint16_t value = 0;
    int i = 0; 
	while(i < nSamples) {
        esp_err_t err = i2s_read(I2S_NUM_0, &rawSample, sampleSize, &bytesRead, portMAX_DELAY);
        if (err != ESP_OK || bytesRead != sampleSize) {
            Log::Error(TAG, "Failed to read I2S ADC sample at index %d", i);
            continue;
        }
		if(rawSample==0){
			continue;
		}
		value = (rawSample & 0x0FFF);
		if(value==0){
			/*ADC is not perfectly rail to rail and never returns 0.
			So if 0 that means buffer has not caught up yet.*/
			continue;
		}
        sum += value;  // Mask to 12 bits
		i++;
    }
    return static_cast<uint16_t>(sum / nSamples);
}
uint16_t AnalogueInputs::waitTillVoltageThresholdExceeded() {
	int totalBufferLength = getTotalBuffersLength();
    const size_t sampleSize = totalBufferLength*sizeof(uint16_t);
    uint16_t rawSample[totalBufferLength];
    size_t bytesRead = 0;
	while(true) {
        esp_err_t err = i2s_read(I2S_NUM_0, &rawSample, sampleSize, 
			&bytesRead, portMAX_DELAY);
        if (err != ESP_OK) {
            Log::Error(TAG, "Failed to read I2S ADC sample");
            return -1;
        }
		if(bytesRead != sampleSize){
            Log::Error(TAG, "Sample size did not match");
            continue;
		}
		for(int i=0; i<totalBufferLength; i++){
			uint16_t value = rawSample[i];
			Log::Info(TAG, "%u", value);
		}
		continue;
		/*
		value = (rawSample & 0x0FFF);
		if(value==0){
			continue;
		}*/
    }
    return static_cast<uint16_t>(0);
}
double AnalogueInputs::singleCorrectedVoltageSampleSelectedI2SChannel(){
	return convertRawToVoltage(singleRawSampleSelectedI2SChannel());
}
double AnalogueInputs::averagedCorrectedVoltageSampleSelectedI2SChannel(){
	return convertRawToVoltage(averagedRawSampleSelectedI2SChannel());
}
double AnalogueInputs::averagedCorrectedVoltageSampleSelectedI2SChannel(int nSamples){
	return convertRawToVoltage(averagedRawSampleSelectedI2SChannel(nSamples));
}
double AnalogueInputs::convertRawToVoltage(uint16_t raw){
	return static_cast<double>(esp_adc_cal_raw_to_voltage(raw, _adc_chars))
		*_correctionFactor/1000.0;
}
uint16_t AnalogueInputs::convertVoltageToApproximateRaw(double voltage){
    double raw = voltage * _voltageToRaw;

    // Clamp to 12-bit range (0–4095)
    if (raw < 0.0) return 0;
    if (raw > 4095.0) return 4095;
    return static_cast<uint16_t>(raw);
}
void AnalogueInputs::selectVoltageDividerInputAsI2SChannel(){
	setChannel(ADC_CHANNEL_VOLTAGE_DIVIDER_INPUT);
}
double AnalogueInputs::getCorrection() {
    return _correctionFactor;
}
