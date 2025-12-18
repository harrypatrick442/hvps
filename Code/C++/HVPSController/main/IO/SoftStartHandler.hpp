#pragma once
#include "../Generated/HVPSConfiguration.hpp"
class SoftStartHandler{
	private:
		static constexpr int SAMPLE_INTERVAL_MS = 10;
		static constexpr int WINDOW_SAMPLES     = 100;
		static constexpr int RELAY_SWITCH_TIME_MILLISECONDS = 100;
		static constexpr int TALK_INTERVAL_MILLISECONDS = 5000;
		static constexpr int MINIMUM_VOLTAGE_TO_REACH = 4;//TODO
		static inline constexpr const char* TAG = "SoftStartHandler";
	public:
		static double doSoftStart(const HVPSConfiguration& config1, const HVPSConfiguration& config2);
	private:
		static const char* tellWhatWaitingOn(bool reachedMinimumVoltage, bool voltageStoppedIncreasing);
};