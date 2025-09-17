#include "ReverseVoltageToRawLookup.hpp"
#include "../Logging/Log.hpp"

const char* ReverseVoltageToRawLookup::TAG = "ReverseVoltageToRawLookup";
/*We are creating a lookup to interpret a voltage to a raw value.
Voltages in between the steps can be used via a linear interpolation.
*/
ReverseVoltageToRawLookup::ReverseVoltageToRawLookup(
	int intervalMillivolts, esp_adc_cal_characteristics_t* adc_chars) {

	nIntervals = (int)ceil(3300.0 / intervalMillivolts) + 1;
	_entries = new ReverseVoltageToRawLookup_Entry[nIntervals];

	for (int i = 0; i < nIntervals; ++i) {
		uint32_t voltage = i * intervalMillivolts;
		uint32_t raw = findRawForVoltage(voltage, adc_chars);
		if(raw>4095){
			Log::Info(TAG, "Out of range raw was returned with value %u", raw);
			abort();
		}
		_entries[i] = ReverseVoltageToRawLookup_Entry(voltage, raw);
	}
}

ReverseVoltageToRawLookup::~ReverseVoltageToRawLookup() {
	delete[] _entries;
}

uint32_t ReverseVoltageToRawLookup::lookupMillivolts(uint32_t voltageMillivolts) {
	// Binary search over precomputed _entries
	uint32_t testVoltage = _entries[0].voltage;
	if(testVoltage >=voltageMillivolts){
		return _entries[0].raw;
	}
	int highIndex = nIntervals - 1;
	testVoltage = _entries[highIndex].voltage;
	if(testVoltage <=voltageMillivolts){
		return _entries[highIndex].raw;
	}
	int lowIndex = 0;
	while (true) {
		int midIndex = (lowIndex + highIndex) / 2;
		if(midIndex==lowIndex||midIndex==highIndex){
			double denominator = (double)(_entries[highIndex].voltage - _entries[lowIndex].voltage);
			if (denominator <= 0) {
				return _entries[lowIndex].raw;
			}
			
			double fraction = (double)(voltageMillivolts - _entries[lowIndex].voltage) / denominator;
			double interpolated = ((double)(_entries[highIndex].raw - _entries[lowIndex].raw) 
				* fraction) + _entries[lowIndex].raw;

			return (uint32_t)interpolated;
		}
		testVoltage = _entries[midIndex].voltage;
		if(testVoltage>voltageMillivolts){
			highIndex = midIndex;
			continue;
		}
		if(testVoltage < voltageMillivolts){
			lowIndex = midIndex;
			continue;
		}
		return _entries[midIndex].raw;
	}
}

uint32_t ReverseVoltageToRawLookup::lookupVolts(double volts) {
	uint32_t millivolts = static_cast<uint32_t>(volts * 1000.0);
	return lookupMillivolts(millivolts);
}
uint32_t ReverseVoltageToRawLookup::findRawForVoltage(
	uint32_t targetVoltageMillivolts,
	esp_adc_cal_characteristics_t* adc_chars) {

	int low = 0;
	int high = 4095;
	bool shiftedHighElseLowToMid = false;
	uint32_t voltage;
	uint32_t lowestVoltageCanReturn = esp_adc_cal_raw_to_voltage(low, adc_chars);
	if(targetVoltageMillivolts<=lowestVoltageCanReturn)
		return low;
	uint32_t highestVoltageCanReturn = esp_adc_cal_raw_to_voltage(high, adc_chars);
	if(targetVoltageMillivolts>=highestVoltageCanReturn)
		return high;
	while (true) {
		int mid = (low + high) / 2;
		if(mid==low||high==mid){
			return low;
		}
		voltage = esp_adc_cal_raw_to_voltage(mid, adc_chars);

		if (voltage < targetVoltageMillivolts) {
			low = mid;
			shiftedHighElseLowToMid = true;
			continue;
		} 
		if(voltage > targetVoltageMillivolts){
			high = mid;
			shiftedHighElseLowToMid = false;
			continue;
		}
		return mid;
	}
}

