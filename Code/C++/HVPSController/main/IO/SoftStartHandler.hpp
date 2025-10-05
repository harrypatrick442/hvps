#pragma once
#include "../Generated/HVPSConfiguration.hpp"
class SoftStartHandler{
	private:
		static constexpr int SAMPLE_INTERVAL_MS = 1;
		static constexpr int WINDOW_SAMPLES     = 100;
		static constexpr int RELAY_SWITCH_TIME_MILLISECONDS = 100;
		static inline constexpr const char* TAG = "SoftStartHandler";
	public:
		static double doSoftStart(const Configuration& config1, const Configuration& config2);
};