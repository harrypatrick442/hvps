#ifndef INPUTS_HPP
#define INPUTS_HPP

#include "driver/gpio.h"
#include "ADC/IADCSession.hpp"
#include <functional>
#include <optional>
class Inputs {
	private:
		static const char* TAG;
		static bool _initialized;
	public:
		static void initialize();
		static void checkInitialized();		
		static void useADCPowerSupplyVoltageFeedbackChannel(const std::function<void(IADCSession&&)>& fn);
};

#endif // INPUTS_HPP
