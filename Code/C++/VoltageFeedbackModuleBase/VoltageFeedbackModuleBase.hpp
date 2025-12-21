#pragma once
#include "Generated/VoltageFeedbackModuleConfiguration.hpp"
#include <functional>
class VoltageFeedbackModuleBase{
	public:
		static void main(
			const VoltageFeedbackModuleConfiguration& config1,
			const VoltageFeedbackModuleConfiguration& config2, 
			std::function<void()> validateConfig);
};