#pragma once
class Outputs{
	private:
		static bool _initialized;
	public:
		static void initialize();
		static void toSafe();
		static void toSafeReversible();
		static void setThresholdReached(bool isReached);
};