#ifndef ADCChannels_hpp
#define ADCChannels_hpp
#include "esp_adc_cal.h"
class ADCChannels{
	public:
        static adc_channel_t REFERENCE_VOLTAGE;
        static adc_channel_t SNUBBER_VOLTAGE_FEEDBACK;
        static adc_channel_t PRIMARY_CURRENT_FEEDBACK;
        static adc_channel_t POWER_SUPPLY_VOLTAGE_FEEDBACK;
};
#endif //ADCChannels_hpp