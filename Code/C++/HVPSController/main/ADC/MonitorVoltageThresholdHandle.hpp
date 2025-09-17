#ifndef MonitorVoltageThresholdHandle_HPP
#define MonitorVoltageThresholdHandle_HPP
#include "ReverseVoltageToRawLookup.hpp"
#include <atomic>
#include <functional>  // for std::function
class MonitorVoltageThresholdHandle{
	private:
		ReverseVoltageToRawLookup* _reverseLookup;
	public:
		volatile uint32_t rawThreshold;
		void setThresholdVoltage(double voltage);
		std::atomic<bool> exit{false};
		std::function<void(bool)> callback;
		void stop();
		MonitorVoltageThresholdHandle(
			double initialVoltage,
			ReverseVoltageToRawLookup* reverseLookup,
			std::function<void(bool)> cb
		);
};
#endif