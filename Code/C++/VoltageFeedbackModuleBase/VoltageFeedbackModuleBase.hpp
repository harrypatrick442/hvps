#pragma once
#include "Generated/VoltageFeedbackModuleConfiguration.hpp"
#include <functional>
class VoltageFeedbackModuleBase{
	private:
		static inline constexpr const char* TAG = "VoltageFeedbackModuleBase";
	public:
		static void main(
			const VoltageFeedbackModuleConfiguration& config1,
			const VoltageFeedbackModuleConfiguration& config2, 
			std::function<void()> validateConfig);
};