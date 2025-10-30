#ifndef INPUTS_HPP
#define INPUTS_HPP
#include "driver/gpio.h"
#include "ADC/IADCSession.hpp"
#include <functional>
class Inputs {
	private:
		static inline constexpr int RECEIVER_INPUT_PIN = 22;
		static inline constexpr const char* TAG = "Inputs";
		static bool _initialized;

	public:
		static void initialize();
		static int readReceiver();
		static void useADCVoltageDividerChannel(const std::function<void(IADCSession&&)>& fn);
};

#endif // INPUTS_HPP
