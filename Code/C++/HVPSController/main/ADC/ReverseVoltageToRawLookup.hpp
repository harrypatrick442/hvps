#ifndef ReverseVoltageToRawLookup_HPP
#define ReverseVoltageToRawLookup_HPP

#include <cmath>
#include <cstdint>
#include "esp_adc_cal.h"
#include "ReverseVoltageToRawLookup_Entry.hpp"

class ReverseVoltageToRawLookup {
private:
    static const char* TAG;
	ReverseVoltageToRawLookup_Entry* _entries;
	int nIntervals;

public:
	ReverseVoltageToRawLookup(int intervalMillivolts, esp_adc_cal_characteristics_t* adc_chars);
	~ReverseVoltageToRawLookup();

	uint32_t lookupMillivolts(uint32_t voltageMillivolts);
	uint32_t lookupVolts(double volts);

private:
	uint32_t findRawForVoltage(uint32_t targetVoltageMillivolts, esp_adc_cal_characteristics_t* adc_chars);
};
#endif
