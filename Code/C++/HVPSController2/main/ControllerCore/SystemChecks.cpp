#include <memory>
#include "SystemChecks.hpp"
#include "../Ports/Port_ControllingMachine.hpp"
#include "Timing/Delay.hpp"
#include "Macros/IncludeLineOnEnd.hpp"
#include "../IO/Inputs.hpp"
std::shared_ptr<SystemChecksResult> SystemChecks::run(){
	std::string errorMessage;
	bool success = run(errorMessage);
	return std::make_shared<SystemChecksResult>(success, errorMessage);
}
bool SystemChecks::run(std::string& errorMessage){
	/*
	if(!suspended_firstStageVoltageFeedbackAbstractComs_highSpeedFeedback(errorMessage)){
		return false;
	}
	if(!suspended_outputVoltageFeedbackAbstractComs_highSpeedFeedback(errorMessage)){
		return false;
	}
	*/
	return true;
}
/*
bool SystemChecks::suspended_firstStageVoltageFeedbackAbstractComs_highSpeedFeedback(
	std::string& errorMessage
){
	return _suspended_voltageFeedbackModule_highsSpeedFeedback(
		FIRST_STAGE_VOLTAGE_FEEDBACK_MODULE_FRIENDLY_NAME,
		errorMessage,
		Port_FirstStageVoltageFeedback::getInstance(),
		Inputs::getFirstStageVoltageFeedbackThresholdReached
	);
}
bool SystemChecks::suspended_outputVoltageFeedbackAbstractComs_highSpeedFeedback(
	std::string& errorMessage
){
	return _suspended_voltageFeedbackModule_highsSpeedFeedback(
		OUTPUT_VOLTAGE_FEEDBACK_MODULE_FRIENDLY_NAME,
		errorMessage,
		Port_OutputVoltageFeedback::getInstance(),
		Inputs::getOutputVoltageFeedbackThresholdReached
	);
}


bool SystemChecks::_suspended_voltageFeedbackAbstractComs_replies( 
	const std::string& moduleFriendlyName,
	std::string& errorMessage,
	Port_VoltageFeedbackBase& port
){
	float voltage;
	if(port.getVoltage(voltage))
	{
		return true;
	}
	errorMessage = "Failed to communicate with "+moduleFriendlyName;
	return false;
}
bool SystemChecks::_suspended_voltageFeedbackModule_highsSpeedFeedback( 
	const std::string& moduleFriendlyName,
	std::string& errorMessage,
	Port_VoltageFeedbackBase& port,
	std::function<bool()> getThresholdReached
){
	bool success = false;
	while(true){
		if(!port.setForceThresholdReachedFeedback(false)){
			errorMessage = INCLUDE_LINE_ON_END("Failed to communicate with "+moduleFriendlyName);
			break;
		}
		if(getThresholdReached()){
			errorMessage = INCLUDE_LINE_ON_END("Threshold shouldnt be in reached state for "+moduleFriendlyName);
			break;
		}
		if(!port.setForceThresholdReachedFeedback(true)){
			errorMessage = INCLUDE_LINE_ON_END("Failed to communicate with "+moduleFriendlyName);
			break;
		}
		if(!getThresholdReached()){
			
			errorMessage = "High speed feedback did not indicate threshold reached during testing "+moduleFriendlyName+" high speed feedback";
			break;
		}
		if(!port.setForceThresholdReachedFeedback(std::nullopt)){
			errorMessage = INCLUDE_LINE_ON_END("Failed to communicate with "+moduleFriendlyName);
			break;
		}
		success = true;
		break;
	}
	port.setForceThresholdReachedFeedback(std::nullopt);
	return success;
}
*/
/*
bool SystemChecks::suspended_firstStageVoltageFeedback_setVoltageThreshold(){
	
}
bool SystemChecks::suspended_outputVoltageFeedback_setVoltageThreshold(){
	
}
bool SystemChecks::suspended_supplyVoltageSuitable(){
	
}
bool singleCycle(){
	
}
bool singleCycle_firstStageVoltageIncreased(){
	
}
bool singleCycle_firstStageVoltageSuitable(){
	
}*/