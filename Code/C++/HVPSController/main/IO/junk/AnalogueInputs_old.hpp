#ifndef ANALOGUE_INPUTS_HPP
#define ANALOGUE_INPUTS_HPP
#define DEFAULT_VREF    1100    // Use ADC calibration eFuse if available, else default to 1.1V
#define ADC_CHANNEL_VOLTAGE_DIVIDER_INPUT     ADC1_CHANNEL_5
#define ADC_ATTEN       ADC_ATTEN_DB_12  // Input range: 0–3.3V
#define ADC_WIDTH       ADC_WIDTH_BIT_12
#define KNOWN_EXTERNAL_REFERENCE_VOLTAGE 2.5     // volts
#define ADC_CHARACTERIZATION_VREF       1100     // mV for esp_adc_cal_characterize()
#define SAMPLE_RATE     100000  // 100 kHz
#define BUF_COUNT     2
#define BUF_LEN     8
#include "driver/i2s.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include <optional>




class AnalogueInputs{
	private:
        static const char* TAG;
		static bool _initialized;
		static esp_adc_cal_characteristics_t *_adc_chars;
		static double _correctionFactor, _voltageToRaw;
		static std::optional<adc1_channel_t> _currentChannel;
		static void setChannel(adc1_channel_t  channel);
	public:
		static int getTotalBuffersLength();
		static void initialize();
		static uint16_t singleRawLatestSampleSelectedI2SChannel();
		static uint16_t singleRawSampleSelectedI2SChannel();
		static uint16_t averagedRawSampleSelectedI2SChannel();
		static uint16_t averagedRawSampleSelectedI2SChannel(int nSamples);
		static double singleCorrectedVoltageSampleSelectedI2SChannel();
		static double averagedCorrectedVoltageSampleSelectedI2SChannel();
		static double averagedCorrectedVoltageSampleSelectedI2SChannel(int nSamples);
		static double convertRawToVoltage(uint16_t voltage);
		static uint16_t convertVoltageToApproximateRaw(double voltage);
		static void selectVoltageDividerInputAsI2SChannel();
		static double getCorrection();
		static uint16_t waitTillVoltageThresholdExceeded();
};
#endif