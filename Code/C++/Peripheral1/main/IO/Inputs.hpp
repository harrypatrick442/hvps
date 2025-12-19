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
		static void configureInputPin(int pin, std::optional<bool> pullDown = std::nullopt);

	public:
		static void initialize();
		static bool getDriveSignal();
		
		static void checkInitialized();
};

#endif // INPUTS_HPP
