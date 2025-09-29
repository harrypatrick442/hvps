#pragma once
class SoftStartHandler{
	private:
		static constexpr int SAMPLE_INTERVAL_MS = 1;
		static constexpr int WINDOW_SAMPLES     = 100;
		static constexpr int RELAY_SWITCH_TIME_MILLISECONDS = 100;
	public:
		static double doSoftStart();
};