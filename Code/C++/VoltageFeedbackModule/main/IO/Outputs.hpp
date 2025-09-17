#ifndef OUTPUTS_HPP
#define OUTPUTS_HPP
class Outputs{
	private:
        static const char* TAG;
		static bool _initialized;
	public:
		static void initialize();
		static void toSafe();
		static void setThresholdReachedFiberOpticOnOff(bool onElseOff);
};
#endif