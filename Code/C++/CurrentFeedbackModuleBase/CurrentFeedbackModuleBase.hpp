#pragma once
#include "Generated/CurrentFeedbackModuleConfiguration.hpp"
class CurrentFeedbackModuleBase{
	private:
		static inline constexpr const char* TAG = "CurrentFeedbackModuleBase";
	public:
		static void main(const Configuration& config1, const Configuration& config2);
};