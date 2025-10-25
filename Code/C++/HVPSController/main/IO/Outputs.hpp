#ifndef OUTPUTS_HPP
#define OUTPUTS_HPP
#include <cstdint>
class Outputs{
	
	private:
        static const char* TAG;
		static bool s_initialized;
		static bool s_safe;
		static void configureOutputPin(int pin, bool onElseOff = false);
		static void _setMOSFETOnOff(bool onElseOff);
		static void _setSoftStartResistorBypassOnOff(bool onElseOff);
	public: 
		static void initialize();
		static void toSafe();
		static void toSafeReversible();
		static void setMOSFETOnOff(bool onElseOff);
		static void setSoftStartResistorBypassOnOff(bool onElseOff);
};
#endif