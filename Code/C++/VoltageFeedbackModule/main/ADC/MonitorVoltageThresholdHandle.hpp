#ifndef MonitorVoltageThresholdHandle_HPP
#define MonitorVoltageThresholdHandle_HPP
#include "ReverseVoltageToRawLookup.hpp"
class MonitorVoltageThresholdHandle{
	private:
		ReverseVoltageToRawLookup* _reverseLookup;
	public:
		volatile uint32_t rawThreshold;
		void setThresholdVoltage(double voltage);
		volatile bool exit;
		void stop();
		MonitorVoltageThresholdHandle(
			double initialVoltage,
			ReverseVoltageToRawLookup* reverseLookup
		);
};
#endif