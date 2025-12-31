#include "HighSpeedCore.hpp"
#include "Tasks/TaskFactory.hpp"
#include "../IO/Inputs.hpp"
#include "../IO/Outputs.hpp"
#include "Timing/Delay.hpp"
#include "Timing/TimeHelper.hpp"
#include "Core/FloatAndTime.hpp"
#include "SystemChecks.hpp"
#include "Macros/GetFileName.hpp"
const char* HighSpeedCore::getTag() {return GET_FILE_NAME;}
HighSpeedCore::HighSpeedCore(
	const HVPSConfiguration& hvpsConfiguration1,
	const HVPSConfiguration& hvpsConfiguration2,
	Port_FirstStageVoltageFeedback& portFirstStageVoltageFeedback, 
	Port_OutputVoltageFeedback& portOutputVoltageFeedback,
	LiveDataCache& liveDataCache,
	bool inError
):
_hvpsConfiguration1(hvpsConfiguration1),
_hvpsConfiguration2(hvpsConfiguration2),
_portFirstStageVoltageFeedback(portFirstStageVoltageFeedback),
_portOutputVoltageFeedback(portOutputVoltageFeedback),
_liveDataCache(liveDataCache),

/*
DO NOT EVER SET _shuttingOrShutDown or _shuttingOrShutDown_2 BACK TO FALSE. EVER!!!!
A FULL SYSTEM REBOOT IS REQUIRED TO ACTIVATE AFTER SHUTDOWN!!
THE ENTIRE POINT OF THIS IS TO PUT THE SYSTEM INTO A PERMANENT STATE WHERE IT CANNOT BE
REACTIVATED WITHOUT REBOOT!!
*/
_shuttingOrShutDown(false),
_actualSystemState(SystemState::Idle),
_desiredSystemState(SystemState::Idle),
_shuttingOrShutDown_2(false),
_inError(inError),
_systemChecksResult(nullptr),
_runSystemChecksLatch(),
_frequencyMeter(){
	startCoreTask();
	_frequencyMeter.startPrintToConsoleLoop();
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
	//lock.unlock();
    return result; // refcount is incremented, safe after unlock
}
void HighSpeedCore::shutDown(){
	setDesiredSystemState(SystemState::ShutDown);
	/*
	DO NOT EVER SET THESE BACK TO FALSE. EVER!!!!
	A FULL SYSTEM REBOOT IS REQUIRED TO ACTIVATE AFTER SHUTDOWN!!
	THE ENTIRE POINT OF THIS IS TO PUT THE SYSTEM INTO A PERMANENT STATE WHERE IT CANNOT BE
	REACTIVATED WITHOUT REBOOT!!
	*/
	_shuttingOrShutDown.store(true, std::memory_order_relaxed);
	_shuttingOrShutDown_2.store(true, std::memory_order_relaxed);
}
void HighSpeedCore::setInError(bool value){
	_inError.store(value, std::memory_order_relaxed);
	if(value){
		setDesiredSystemState(SystemState::Error);
		return;
	}
	if(isShuttingDownOrShutDown()){
		setActualSystemState(SystemState::ShuttingDown);
		setDesiredSystemState(SystemState::ShuttingDown);
		return;
	}
	setDesiredSystemState(SystemState::Idle);
	setActualSystemState(SystemState::Idle);
}
bool HighSpeedCore::getInError(){
	return _inError.load(std::memory_order_relaxed);
}
SystemState HighSpeedCore::getDesiredSystemState(){
	return _desiredSystemState.load(std::memory_order_relaxed);
}
SystemState HighSpeedCore::getActualSystemState(){
	return _actualSystemState.load(std::memory_order_relaxed);
}
void HighSpeedCore::setDesiredSystemState(SystemState systemState){
	_desiredSystemState.store(systemState, std::memory_order_relaxed);
}
void HighSpeedCore::setActualSystemState(SystemState systemState) {
	SystemState old = _actualSystemState.exchange(systemState, std::memory_order_relaxed);
	if(old != systemState){
        dispatchSystemStateChanged(systemState);
	}
}
bool HighSpeedCore::isShuttingDownOrShutDown(){
	
	/*
	DO NOT EVER SET _shuttingOrShutDown or _shuttingOrShutDown_2 BACK TO FALSE. EVER!!!!
	A FULL SYSTEM REBOOT IS REQUIRED TO ACTIVATE AFTER SHUTDOWN!!
	THE ENTIRE POINT OF THIS IS TO PUT THE SYSTEM INTO A PERMANENT STATE WHERE IT CANNOT BE
	REACTIVATED WITHOUT REBOOT!!
	*/
	if(_shuttingOrShutDown.load(std::memory_order_relaxed)){
		return true;
	}
	if(_shuttingOrShutDown_2.load(std::memory_order_relaxed)){
		return true;
	}
	return false;
}
void HighSpeedCore::startCoreTask(){
	TaskFactory::createPriorityTask([this](){
		_run();
	}, "HighSpeedCore::_run");
}
void HighSpeedCore::_run(){
	while(true){
		Delay::ms(100);
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
				Outputs::setMOSFETOnOff(false);
				//Second set for backup
				continue;
			case SystemState::ShutDown:
				LOG_INFO("ShutDown");
				doShutDown();
				continue;
			case SystemState::RunningSystemChecks:
				doRunningSystemChecks();
				continue;
			case SystemState::Error:
				LOG_INFO("Error");
				doError();
				continue;
			default:
				SAFE_ABORT("Illegal state");
				break;
				
		}
	}
}
void HighSpeedCore::doRunningSystemChecks(){
	setDesiredSystemState(doSystemChecks()->getSuccess()?SystemState::Idle:SystemState::Error);
}
std::shared_ptr<SystemChecksResult> HighSpeedCore::doSystemChecks(){
	dispatchMessage("Running system checks!");
	setActualSystemState(SystemState::RunningSystemChecks);
	std::shared_ptr<SystemChecksResult> result = SystemChecks::run();
	std::unique_lock<std::mutex> lock(_mutexSystemChecksResult);
	_systemChecksResult = result;
	_runSystemChecksLatch.unlatch();
	if(!result->getSuccess()){
		setInError(true);//NOTE THIS ACTUALLY CLEARS THE _runSystemChecksLatch too.
		dispatchError(result->getErrorMessage());//TODO THIS ISNT RIGHT 10/11/2025
		dispatchMessage("Failed system checks :(");
	}
	else{
		setActualSystemState(SystemState::Idle);
		dispatchMessage("Passed system checks! :)");
	}
	return result;
}
void HighSpeedCore::doShutDown(){
	bool v = true;
	
	/*
	DO NOT EVER SET _shuttingOrShutDown or _shuttingOrShutDown_2 BACK TO FALSE. EVER!!!!
	A FULL SYSTEM REBOOT IS REQUIRED TO ACTIVATE AFTER SHUTDOWN!!
	THE ENTIRE POINT OF THIS IS TO PUT THE SYSTEM INTO A PERMANENT STATE WHERE IT CANNOT BE
	REACTIVATED WITHOUT REBOOT!!
	*/
	_shuttingOrShutDown.store(v, std::memory_order_relaxed);
	_shuttingOrShutDown_2.store(v, std::memory_order_relaxed);
	setActualSystemState(SystemState::ShuttingDown);
	uint64_t lastTime = 0;
	SystemState desiredSystemState = getDesiredSystemState();
	while(true){
		LOG_INFO("Doing shut down");
		if(
			(desiredSystemState!=SystemState::ShutDown)
			&&
			(desiredSystemState!=SystemState::ShuttingDown))
		{
			return;
		}
		Outputs::setMOSFETOnOff(false);
		FloatAndTime outputVoltageAndTime = _liveDataCache.getOutputVoltage();
		LOG_INFO("Output voltage is: %f", outputVoltageAndTime.f);
		LOG_INFO("SAFE_OUTPUT_VOLTAGE is: %f", SAFE_OUTPUT_VOLTAGE);
		if(outputVoltageAndTime.t!=lastTime){
			if(outputVoltageAndTime.f<=SAFE_OUTPUT_VOLTAGE){
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
	if(getInError()){
		return;
	}
	std::shared_ptr<SystemChecksResult> systemChecksResult = doSystemChecks();
	if(!systemChecksResult->getSuccess()){
		return;
	}
	if(getDesiredSystemState()!=SystemState::Live){
		return;
	}
	dispatchMessage("Going live!");
	uint64_t timeUs, endTime, endTime_2;
	setActualSystemState(SystemState::Live);
	timeUs = TimeHelper::us();
	while(true){
		endTime = timeUs+_hvpsConfiguration1.onTimeMicroSeconds;
		endTime_2 = timeUs+_hvpsConfiguration2.onTimeMicroSeconds;
		if((!Inputs::getOutputVoltageFeedbackThresholdReached())&&
		(!Inputs::getFirstStageVoltageFeedbackThresholdReached())){
				Outputs::setMOSFETOnOff(true);
		}
		while(true){
			timeUs = TimeHelper::us();
			if(timeUs>=endTime){
				break;
			}
			if(timeUs>=endTime_2){
				break;
			}
		}
		Outputs::setMOSFETOnOff(false);
		endTime = timeUs+_hvpsConfiguration1.offTimeMicroSeconds;
		endTime_2 = timeUs+_hvpsConfiguration2.offTimeMicroSeconds;
		
		if(getDesiredSystemState()!=SystemState::Live){
			return;
		}
		if(isShuttingDownOrShutDown()){
			return;
		}
		if(getInError()){
			return;
		}
		setActualSystemState(SystemState::Live);
		_frequencyMeter.tick();
		while(true){
			timeUs = TimeHelper::us();
			if(timeUs>=endTime){
				break;
			}
			if(timeUs>=endTime_2){
				break;
			}
		}
	}
}
void HighSpeedCore::doError(){
	setActualSystemState(SystemState::Error);
	while(true){
		Outputs::setMOSFETOnOff(false);
		Delay::ms(100);
		if(getActualSystemState()!=SystemState::Error){
			break;
		}
		if(getDesiredSystemState()==SystemState::RunningSystemChecks){
			break;
		}
	}
}
void HighSpeedCore::dispatchSystemStateChanged(SystemState v){
	onSystemStateChanged.dispatch(v);
}
void HighSpeedCore::dispatchError(std::string errorMessage){
	onError.dispatch(errorMessage);
}
void HighSpeedCore::dispatchMessage(std::string message){
	onMessage.dispatch(message);
}