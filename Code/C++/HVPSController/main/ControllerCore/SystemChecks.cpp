#include <memory>
#include "SystemChecks.hpp"
#include "../Ports/Port_VoltageFeedbackBase.hpp"
#include "../Ports/Port_OutputVoltageFeedback.hpp"
#include "../Ports/Port_FirstStageVoltageFeedback.hpp"
#include "../Ports/Port_ControllingMachine.hpp"
#include "../Ports/Port_OtherPeripherals.hpp"
#include "../IO/Inputs.hpp"
std::shared_ptr<SystemChecksResult> SystemChecks::run(){
	std::string errorMessage;
	bool success = run(errorMessage);
	return std::make_shared<SystemChecksResult>(success, errorMessage);
}
bool SystemChecks::run(std::string& errorMessage){
	if(Port_OtherPeripherals::getInstance().sendIndicateStateRequest()){
		errorMessage= "Failed to speak to peripherals";
		return false;
	}
	uint32_t subsystemIdentifierWithError = 
		Port_ControllingMachine::getInstance().greetVoltageFeedbackModules();
	if(subsystemIdentifierWithError!=0){
		errorMessage = "Error state detected for subsystem "
		+std::to_string(subsystemIdentifierWithError);
		return false;
	}
	if(!suspended_firstStageVoltageFeedbackAbstractComs_replies(errorMessage)){
		return false;
	}
	if(!suspended_outputVoltageFeedbackAbstractComs_replies(errorMessage)){
		return false;
	}
	if(!suspended_firstStageVoltageFeedbackAbstractComs_highSpeedFeedback(errorMessage)){
		return false;
	}
	if(!suspended_outputVoltageFeedbackAbstractComs_highSpeedFeedback(errorMessage)){
		return false;
	}
	return true;
}

bool SystemChecks::suspended_firstStageVoltageFeedbackAbstractComs_replies(
	std::string& errorMessage
){
	return _suspended_voltageFeedbackAbstractComs_replies(
		FIRST_STAGE_VOLTAGE_FEEDBACK_MODULE_FRIENDLY_NAME,
		errorMessage,
		Port_FirstStageVoltageFeedback::getInstance()
	);
}
bool SystemChecks::suspended_outputVoltageFeedbackAbstractComs_replies(
	std::string& errorMessage
){
	return _suspended_voltageFeedbackAbstractComs_replies(
		OUTPUT_VOLTAGE_FEEDBACK_MODULE_FRIENDLY_NAME,
		errorMessage,
		Port_OutputVoltageFeedback::getInstance()
	);
}
bool SystemChecks::suspended_firstStageVoltageFeedbackAbstractComs_highSpeedFeedback(
	std::string& errorMessage
){
	return _suspended_voltageFeedbackModule_highsSpeedFeedback(
		FIRST_STAGE_VOLTAGE_FEEDBACK_MODULE_FRIENDLY_NAME,
		errorMessage,
		Inputs::getFirstStageVoltageFeedbackThresholdReached
	);
}
bool SystemChecks::suspended_outputVoltageFeedbackAbstractComs_highSpeedFeedback(
	std::string& errorMessage
){
	return _suspended_voltageFeedbackModule_highsSpeedFeedback(
		OUTPUT_VOLTAGE_FEEDBACK_MODULE_FRIENDLY_NAME,
		errorMessage,
		Inputs::getOutputVoltageFeedbackThresholdReached
	);
}


bool SystemChecks::_suspended_voltageFeedbackAbstractComs_replies( 
	const std::string& moduleFriendlyName,
	std::string& errorMessage,
	Port_VoltageFeedbackBase& port
){
	double voltage;
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
	std::function<bool()> getThresholdReached
){
	
	if(!Port_OutputVoltageFeedback::getInstance().setForceThresholdReachedFeedback(true)){
		errorMessage = "Failed to communicate with "+moduleFriendlyName+" during testing high speed feedback (setting forced on)";
		return false;
	}
	if(!getThresholdReached()){
		errorMessage = "High speed feedback did not indicate threshold reached during testing "+moduleFriendlyName+" high speed feedback";
		return false;
	}
	if(!Port_OutputVoltageFeedback::getInstance().setForceThresholdReachedFeedback(false)){
		errorMessage = "Failed to communicate with "+moduleFriendlyName+" during testing high speed feedback (setting forced off)";
		return false;
	}
	if(getThresholdReached()){
		errorMessage = "High speed feedback did not indicate threshold not reached during testing "+moduleFriendlyName
			+" high speed feedback. Appears stuck on. This can result from fiber optic disconnection";
		return false;
	}
	return true;
}

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