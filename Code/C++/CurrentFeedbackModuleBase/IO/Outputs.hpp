#pragma once
class Outputs{
	private:
		static inline constexpr const char* TAG = "Outputs";
		static bool _initialized;
	public:
		static void initialize();
		static void toSafe();
		static void toSafeReversible();
		static void setThresholdReachedFiberOpticOnOff(bool onElseOff);
};