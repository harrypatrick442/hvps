#include "HighSpeedCore.hpp"
#include "Tasks/TaskFactory.hpp"
#include "../IO/Inputs.hpp"
#include "../IO/Outputs.hpp"
#include "Timing/Delay.hpp"
#include "Timing/TimeHelper.hpp"
#include "Core/FloatAndTime.hpp"
#include "SystemChecks.hpp"
HighSpeedCore::HighSpeedCore(
	Port_FirstStageVoltageFeedback& portFirstStageVoltageFeedback, 
	Port_OutputVoltageFeedback& portOutputVoltageFeedback,
	LiveDataCache& liveDataCache,
	bool inError
):
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
_runSystemChecksLatch(){
	startCoreTask();
}

void HighSpeedCore::start(){
	setDesiredSystemState(SystemState::Live);
}
void HighSpeedCore::stop(){
	setDesiredSystemState(SystemState::Idle);
}
std::shared_ptr<SystemChecksResult> HighSpeedCore::runSystemChecksOnly(){
	Log::Info(TAG, "runSystemChecksOnly...");
	_runSystemChecksLatch.latch();
	Log::Info(TAG, "runSystemChecksOnly...2");
	setDesiredSystemState(SystemState::RunningSystemChecks);
	Log::Info(TAG, "runSystemChecksOnly...3");
	_runSystemChecksLatch.wait();
	Log::Info(TAG, "runSystemChecksOnly...4");
	std::unique_lock<std::mutex> lock(_mutexSystemChecksResult);
	Log::Info(TAG, "runSystemChecksOnly...5");
    auto result = _systemChecksResult; // copy under lock
	Log::Info(TAG, "runSystemChecksOnly...6");
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
		Log::Info(TAG, "looping...");
		Delay::ms(100);
		if(isShuttingDownOrShutDown()||getActualSystemState()==SystemState::ShutDown){
			Log::Info(TAG, "Is shut down");
			doShutDown();
			continue;
		}
		switch(getDesiredSystemState()){
			case SystemState::Idle:
				Log::Info(TAG, "Idle");
				doIdle();
				continue;
			case SystemState::Live:
				Log::Info(TAG, "Live");
				doLive();
				Outputs::setMOSFETOnOff(false);
				//Second set for backup
				continue;
			case SystemState::ShutDown:
				Log::Info(TAG, "ShutDown");
				doShutDown();
				continue;
			case SystemState::RunningSystemChecks:
				Log::Info(TAG, "RunningSystemChecks");
				doSystemChecks();
				continue;
			case SystemState::Error:
				Log::Info(TAG, "Error");
				doError();
				continue;
			default:
				Aborter::safeAbort(TAG, "Illegal state");
				break;
				
		}
	}
}
std::shared_ptr<SystemChecksResult> HighSpeedCore::doSystemChecks(){
	Log::Info(TAG, "doSystemChecks");
	std::shared_ptr<SystemChecksResult> result = SystemChecks::run();
	std::unique_lock<std::mutex> lock(_mutexSystemChecksResult);
	_systemChecksResult = result;
	_runSystemChecksLatch.unlatch();
	if(!result->getSuccess()){
		setInError(true);//NOTE THIS ACTUALLY CLEARS THE _runSystemChecksLatch too.
		dispatchError(result->getErrorMessage());//TODO THIS ISNT RIGHT 10/11/2025
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
		if(
			(desiredSystemState!=SystemState::ShutDown)
			&&
			(desiredSystemState!=SystemState::ShuttingDown))
		{
			return;
		}
		Outputs::setMOSFETOnOff(false);
		FloatAndTime outputVoltageAndTime = _liveDataCache.getOutputVoltage();
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
	uint64_t time, endTime, endTime_2;
	setActualSystemState(SystemState::Live);
	while(true){
		endTime = TimeHelper::us()+ON_TIME_US;
		endTime_2 = TimeHelper::us()+ON_TIME_US_2;
		if((!Inputs::getOutputVoltageFeedbackThresholdReached())&&
		(!Inputs::getFirstStageVoltageFeedbackThresholdReached())){
				Outputs::setMOSFETOnOff(true);
		}
		while(true){
			time = TimeHelper::us();
			if(time>=endTime){
				break;
			}
			if(time>=endTime_2){
				break;
			}
		}
		Outputs::setMOSFETOnOff(false);
		
		
		endTime = TimeHelper::us()+OFF_TIME_US;
		endTime_2 = TimeHelper::us()+OFF_TIME_US_2;
		
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
		while(true){
			time = TimeHelper::us();
			if(time>=endTime){
				break;
			}
			if(time>=endTime_2){
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