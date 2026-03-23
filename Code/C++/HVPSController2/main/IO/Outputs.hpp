#pragma once
#include <cstdint>
#include "esp_attr.h"
class Outputs{
	
	private:
        static const char* TAG;
		static bool s_initialized;
		static bool s_safe;
		static void configureOutputPin(int pin, bool onElseOff = false);
		static void _setSoftStartResistorBypassOnOff(bool onElseOff);
	public: 
		static void initialize();
		static void toSafe();
		static void toSafeReversible();
		static void setSoftStartResistorBypassOnOff(bool onElseOff);
};