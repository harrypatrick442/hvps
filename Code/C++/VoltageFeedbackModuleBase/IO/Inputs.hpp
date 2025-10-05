#ifndef INPUTS_HPP
#define INPUTS_HPP

#include "driver/gpio.h"

class Inputs {
	private:
		static const int RECEIVER_INPUT_PIN = 22;
		static const char* TAG;
		static bool _initialized;

	public:
		static void initialize();
		static int readReceiver();
		static void selectADCVoltageDividerInputAsChannel();
		static double getADCVoltage();
};

#endif // INPUTS_HPP
