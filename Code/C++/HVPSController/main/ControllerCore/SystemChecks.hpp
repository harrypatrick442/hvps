#pragma once
#include "SystemChecksResult.hpp"
#include "../Ports/Port_VoltageFeedbackBase.hpp"
#include <functional>
#include <string> 
class SystemChecks{
	public:
		static std::shared_ptr<SystemChecksResult> run();
		static bool run(std::string& errorMessage);
	private:
		static inline constexpr const char* FIRST_STAGE_VOLTAGE_FEEDBACK_MODULE_FRIENDLY_NAME= "First Stage Voltage Feedback Module";
		static inline constexpr const char* OUTPUT_VOLTAGE_FEEDBACK_MODULE_FRIENDLY_NAME= "Output Voltage Feedback Module";
		static bool suspended_firstStageVoltageFeedbackAbstractComs_replies(
			std::string& errorMessage);
		static bool suspended_outputVoltageFeedbackAbstractComs_replies(
			std::string& errorMessage);
		static bool suspended_firstStageVoltageFeedbackAbstractComs_highSpeedFeedback(
			std::string& errorMessage);
		static bool suspended_outputVoltageFeedbackAbstractComs_highSpeedFeedback(
			std::string& errorMessage);
		static bool _suspended_voltageFeedbackAbstractComs_replies( 
			const std::string& moduleFriendlyName,
			std::string& errorMessage,
			Port_VoltageFeedbackBase& port
		);
		static bool _suspended_voltageFeedbackModule_highsSpeedFeedback(
			const std::string& moduleFriendlyName,
			std::string& errorMessage,
			std::function<bool()> getThresholdReached
		);
		
};