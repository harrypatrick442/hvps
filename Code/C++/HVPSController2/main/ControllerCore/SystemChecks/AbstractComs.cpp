#pragma once
#include "SystemChecks.hpp"
SystemChecksResult SystemChecks::run(){
	return std::make_shared<SystemChecksResult>(true);
}
bool SystemChecks::firstStageVoltageFeedbackAbstractComs_Replies(){
	
}
bool SystemChecks::outputVoltageFeedbackAbstractComs_Replies(){
	
}
bool SystemChecks::firstStageVoltageFeedback_SetVoltageThreshold(){
	
}
bool SystemChecks::outputVoltageFeedback_SetVoltageThreshold(){
	
}
bool SystemChecks::firstStageVoltageFeedbackThresholdReached_Changed(){
	
}
bool SystemChecks::outputVoltageFeedbackThresholdReachedWorking(){
	
}
bool SystemChecks::supplyVoltageSuitable(){
	
}
bool singleCycle(){
	
}
bool singleCycle_firstStageVoltageIncreased(){
	
}
bool singleCycle_firstStageVoltageSuitable(){
	
}