#ifndef INPUTS_HPP
#define INPUTS_HPP

#include "driver/gpio.h"
#include <optional>
class Inputs {
	private:
		static const char* TAG;
		static bool _initialized;
		static void configureInputPin(int pin, std::optional<bool> pullDown = std::nullopt);

	public:
		static void initialize();
		static bool getOutputVoltageFeedbackThresholdReached();
		static bool getFirstStageVoltageFeedbackThresholdReached();
		static bool getOutputCurrentFeedbackThresholdReached();
		static void checkInitialized();
};

#endif // INPUTS_HPP
