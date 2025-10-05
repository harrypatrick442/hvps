#pragma once
#include "Generated/VoltageFeedbackModuleConfiguration.hpp"
class VoltageFeedbackModuleBase{
	private:
		static inline constexpr const char* TAG = "VoltageFeedbackModuleBase";
	public:
		static void main(const Configuration& config1, const Configuration& config2);
};