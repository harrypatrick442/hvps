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
		static bool getOutputVoltageFeedbackThresholdReached();
		static bool getFirstStageVoltageFeedbackThresholdReached();
		static bool getOutputCurrentFeedbackThresholdReached();
		static void checkInitialized();
		
		
		static void useADCReferenceVoltageChannel(const std::function<void(IADCSession&&)>& fn);
		static void useADCSnubberVoltageFeedbackChannel(const std::function<void(IADCSession&&)>& fn);
		static void useADCPrimaryCurrentFeedbackChannel(const std::function<void(IADCSession&&)>& fn);
		static void useADCPowerSupplyVoltageFeedbackChannel(const std::function<void(IADCSession&&)>& fn);
};

#endif // INPUTS_HPP
