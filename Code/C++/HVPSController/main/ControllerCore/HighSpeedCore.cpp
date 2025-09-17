#include "HighSpeedCore.hpp"
#include "../Tasks/TaskFactory.hpp"
#include "../IO/Inputs.hpp"
#include "../IO/Outputs.hpp"
HighSpeedCore::HighSpeedCore(
	Port_FirstStageVoltageFeedback& portFirstStageVoltageFeedback, 
	Port_OutputVoltageFeedback& portOutputVoltageFeedback
):
_portFirstStageVoltageFeedback(portFirstStageVoltageFeedback),
_portOutputVoltageFeedback(portOutputVoltageFeedback),
_shuttingOrShutDown(false),
_systemState(SystemState::Idle),
_desiredSystemState(SystemState::Idle),
_shuttingOrShutDown_2(false),
_systemChecksResult(nullptr),
_runSystemChecksLatch(){
	
}

void HighSpeedCore::start(){
	setDesiredSystemState(SystemState::Live);
}
void HighSpeedCore::stop(){
	setDesiredSystemState(SystemState::Idle);
}
std::shared_ptr<SystemChecksResult> HighSpeedCore::runSystemChecksOnly(){
	_runSystemChecksLatch.latch();
	setDesiredSystemState(SystemState::RunningSystemChecks);
	_runSystemChecksLatch.wait();
	std::unique_lock<std::mutex> lock(_mutexSystemChecksResult);
    auto result = _systemChecksResult; // copy under lock
    return result; // refcount is incremented, safe after unlock
	
}
void HighSpeedCore::shutDown(){
	setDesiredSystemState(SystemState::ShutDown);
	_shuttingOrShutDown.store(v, std::memory_order_relaxed);
	_shuttingOrShutDown_2.store(v, std::memory_order_relaxed);
}
SystemState HighSpeedCore::getDesiredSystemState(){
	return _desiredSystemState.load(std::memory_order_relaxed);
}
void HighSpeedCore::setDesiredSystemState(SystemState v){
	_desiredSystemState.store(v, std::memory_order_relaxed);
}
SystemState HighSpeedCore::getActualSystemState(){
	return _actualSystemState.load(std::memory_order_relaxed);
}
void HighSpeedCore::setActualSystemState(SystemState v){
	_actualSystemState.store(v, std::memory_order_relaxed);
	dispatchSystemStateChanged(v);
}
bool HighSpeedCore::isShuttingDownOrShutDown(){
	if(_shuttingOrShutDown.load(std::memory_order_relaxed)){
		return true;
	}
	if(_shuttingOrShutDown_2.load(std::memory_order_relaxed)){
		return true;
	}
	return false;
}
void HighSpeedCore::startCoreTask(){
	TaskFactory::createPriorityTask(_run, nullptr, "HighSpeedCore::_run");
}
void HighSpeedCore::_run(void* ignore){
	while(true){
		Delay::ms(10);
		if(isShuttingDownOrShutDown()||getActualSystemState()==SystemState::ShutDown){
			doShutDown();
			continue;
		}
		switch(getDesiredSystemState()){
			case SystemState::Idle:
				doIdle();
				continue;
			case SystemState::Live:
				doLive();
				continue;
			case SystemState::ShutDown:
				doShutDown();
				continue;
			case SystemState::RunningSystemChecks:
				doSystemChecks();
				continue;
			default:
				Aborter::safeAbort(TAG, "Illegal state");
				break;
				
		}
	}
}
std::shared_ptr<SystemChecksResult> HighSpeedCore::doSystemChecks(){
	std::shared_ptr<SystemChecksResult> result = SystemChecks::run();
	std::unique_lock<std::mutex> lock(_mutexSystemChecksResult);
	_systemChecksResult = result;
}
void HighSpeedCore::doShutDown(){
	bool v = true;
	_shuttingOrShutDown.store(v, std::memory_order_relaxed);
	_shuttingOrShutDown_2.store(v, std::memory_order_relaxed);
	setActualSystemState(SystemState::ShuttingDown);
	uint64_t lastTime = 0;
	while(true){
		if(
			(desiredSystemState!=SystemState::ShutDown)
			&&
			(desiredSystemState!=SystemState::ShuttingDown))
		{
			return;
		}
		DoubleAndTime outputVoltageAndTime = LiveDataCache::getVoltage(outputVoltage);
		if(outputVoltageAndTime.t!=lastTime){
			if(outputVoltageAndTime.d<=SAFE_OUTPUT_VOLTAGE){
				setActualSystemState(SystemState::ShutDown);
			}
			else{
				setActualSystemState(SystemState::ShuttingDown);
			}
			lastTime = outputVoltageAndTime.t;
		}
		Delay::ms(500);
	}
}
void HighSpeedCore::doIdle(){
	while(true){
		setActualSystemState(SystemState::Idle);
		SystemState desiredSystemState = getDesiredSystemState();
		if(desiredSystemState!=SystemState::Idle){
			return;
		}
		Outputs::setMOSFETOnOff(false);
		Delay::ms(100);
	}
}
void HighSpeedCore::doLive(){
	if(isShuttingDownOrShutDown()){
		return;
	}
	std::shared_ptr<SystemChecksResult> systemChecksResult = doSystemChecks();
	if(!systemChecksResult.getSuccess()){
		return;
	}
	uint64_t time, endTime, endTime_2;
	setActualSystemState(SystemState::Live);
	while(true){
		endTime = esp_timer_get_time()+ON_TIME_US;
		endTime_2 = esp_timer_get_time()+ON_TIME_US_2;
		if(isShuttingDownOrShutDown()){
			return;
		}
		if((!Inputs::getOutputVoltageFeedbackThresholdReached())&&
		(!Inputs::getFirstStageVoltageFeedbackThresholdReached())){
				Outputs::setMOSFETOnOff(true);
		}
		while(true){
			time = esp_timer_get_time();
			if(time>=endTime){
				break;
			}
			if(time>=endTime_2){
				break;
			}
		}
		Outputs::setMOSFETOnOff(false);
		
		
		endTime = esp_timer_get_time()+OFF_TIME_US;
		endTime_2 = esp_timer_get_time()+OFF_TIME_US_2;
		
		if(getDesiredSystemState()!=SystemState::Live){
			return;
		}
		setActualSystemState(SystemState::Live);
		
		while(true){
			time = esp_timer_get_time();
			if(time>=endDriveOnTime){
				break;
			}
			if(time>=endTime_2){
				break;
			}
		}
	}
}
void dispatchSystemStateChanged(SystemState v){
	onSystemStateChanged.dispatch(v);
}