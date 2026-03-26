#include "HighSpeedCore.hpp"
#include "Tasks/TaskFactory.hpp"
#include "../IO/Inputs.hpp"
#include "../IO/Outputs.hpp"
#include "Timing/Delay.hpp"
#include "Timing/TimeHelper.hpp"
#include "Core/FloatAndTime.hpp"
#include "SystemChecks.hpp"
#include "Macros/GetFileName.hpp"
#include "ADC/ADC.hpp"
#include <cmath>
const char* HighSpeedCore::getTag() {return GET_FILE_NAME;}
HighSpeedCore::HighSpeedCore(
	const HVPSConfiguration& hvpsConfiguration1,
	const HVPSConfiguration& hvpsConfiguration2,
	IFPGABus& fpgaBus,
	bool inError
):
_hvpsConfiguration1(hvpsConfiguration1),
_hvpsConfiguration2(hvpsConfiguration2),
_fpgaInterface(fpgaBus),

/*
DO NOT EVER SET _shuttingOrShutDown or _shuttingOrShutDown_2 BACK TO FALSE. EVER!!!!
A FULL SYSTEM REBOOT IS REQUIRED TO ACTIVATE AFTER SHUTDOWN!!
THE ENTIRE POINT OF THIS IS TO PUT THE SYSTEM INTO A PERMANENT STATE WHERE IT CANNOT BE
REACTIVATED WITHOUT REBOOT!!
*/
_shuttingOrShutDown(false),
_shuttingOrShutDown_2(false),
_actualSystemState(SystemState::Idle),
_desiredSystemState(SystemState::Idle),
_inError(inError),
_systemChecksResult(nullptr),
_runSystemChecksLatch(),
_startLiveTimeUs(0),
_nCyclesCount(0),
_peakCurrentSenseVoltageRaw(0)
{
	startCoreTask();
}
float HighSpeedCore::getFrequencyHz(ValueBoundType& valueBoundType){
	uint64_t nCyclesCount = _nCyclesCount;
	uint64_t startLiveTimeUs = _startLiveTimeUs;
	uint64_t nowUs = TimeHelper::us();
	if(startLiveTimeUs==0){
		valueBoundType = ValueBoundType::Unknown;
		return 0;
	}
	uint64_t dtUs = nowUs - startLiveTimeUs;
	if(dtUs==0){
		valueBoundType = ValueBoundType::Unknown;
		return 0;
	}
	if(nCyclesCount < MIN_CYCLES_FOR_EXACT
		||dtUs < MIN_DT_US_FOR_EXACT){
		valueBoundType = ValueBoundType::Approximate;
	}
	else{
		valueBoundType = ValueBoundType::Exact;
	}
	return static_cast<float>(nCyclesCount)
		*(1000000.0f/static_cast<float>(dtUs));
}
float HighSpeedCore::getActualPeakPrimaryCurrent(){
	uint8_t raw = _fpgaInterface.getActualPeakPrimaryCurrent();
	return _hvpsConfiguration1.primaryCurrentFromRaw * static_cast<float>(raw);
}
float HighSpeedCore::getActualOutputVoltage(){
	uint8_t raw = _fpgaInterface.getActualOutputVoltage();
	return _hvpsConfiguration1.outputVoltageFromRaw * static_cast<float>(raw);
}
float HighSpeedCore::getActualFirstStageVoltage(){
	uint8_t raw = _fpgaInterface.getActualFirstStageVoltage();
	return _hvpsConfiguration1.firstStageVoltageFromRaw * static_cast<float>(raw);
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
				_fpgaInterface.setDrive(false);
				_fpgaInterface.setDrive2(false);
				continue;
			case SystemState::ShutDown:
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
		dispatchMessage("Failed system checks :(");
		dispatchError(result->getErrorMessage());//TODO THIS ISNT RIGHT 10/11/2025
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
	uint64_t timeAtWhichClassifiedSafeSeconds_1 = 0, 
		timeAtWhichClassifiedSafeSeconds_2 = 0;
	setActualSystemState(SystemState::ShuttingDown);
	bool issuedWarning = false;
	while(true){
		Delay::ms(500);
		if(
			(desiredSystemState!=SystemState::ShutDown)
			&&
			(desiredSystemState!=SystemState::ShuttingDown))
		{
			return;
		}
		_fpgaInterface.setDrive(false);
		_fpgaInterface.setDrive2(false);
		uint64_t fpgaUpdateTime = _fpgaInterface.getLastUpdateTimeUs();
		if(fpgaUpdateTime==lastTime){
			continue;
		}
		float outputVoltage = getActualOutputVoltage();
		lastTime = fpgaUpdateTime;
		if(outputVoltage>SAFE_OUTPUT_VOLTAGE){
			timeAtWhichClassifiedSafeSeconds_1=0;
			timeAtWhichClassifiedSafeSeconds_2=0;
			continue;
		}
		if(_hvpsConfiguration1.outputVoltageFromRaw*2.0f<=SAFE_OUTPUT_VOLTAGE
		||_hvpsConfiguration2.outputVoltageFromRaw*2.0f<=SAFE_OUTPUT_VOLTAGE){
			setActualSystemState(SystemState::ShutDown);
			continue;
		}
		if(
			(timeAtWhichClassifiedSafeSeconds_1==0
			||timeAtWhichClassifiedSafeSeconds_2==0)
			)
		{				
			float additionalTimeRequiredToDischargeSeconds_1;	
			float additionalTimeRequiredToDischargeSeconds_2;
			//TODO not everything is bit flip safe
			calculateAdditionalShutdownTime(
				_hvpsConfiguration1.outputVoltageFromRaw*2.0f,
					additionalTimeRequiredToDischargeSeconds_1,
					additionalTimeRequiredToDischargeSeconds_2);
			LOG_INFO("additionalTimeRequiredToDischargeSeconds: %f", additionalTimeRequiredToDischargeSeconds_1);
			float nowSeconds = static_cast<float>(TimeHelper::s());
			timeAtWhichClassifiedSafeSeconds_1 = (2.0f*additionalTimeRequiredToDischargeSeconds_1)+nowSeconds;
			timeAtWhichClassifiedSafeSeconds_2 = (2.0f*additionalTimeRequiredToDischargeSeconds_2)+nowSeconds;
			if(timeAtWhichClassifiedSafeSeconds_1!=timeAtWhichClassifiedSafeSeconds_2){
				timeAtWhichClassifiedSafeSeconds_1 = 0;
				timeAtWhichClassifiedSafeSeconds_2 = 0;
			}
		}
		if(
			(
				(timeAtWhichClassifiedSafeSeconds_1>0)
				&&
				(timeAtWhichClassifiedSafeSeconds_1<=TimeHelper::s())
			)
			&&
			(
				(timeAtWhichClassifiedSafeSeconds_2>0)
				&&
				(timeAtWhichClassifiedSafeSeconds_2<=TimeHelper::s())
			)){
			setActualSystemState(SystemState::ShutDown);
			if(!issuedWarning){
				dispatchWarning("The last portion of the shutdown period had to be approximated due to limited accuracy of voltage feedback. You may now HOTSTICK the device fitting the grounding connector");
				issuedWarning = true;
			}
		}
	}
}
void HighSpeedCore::doIdle(){
	while(true){
		setActualSystemState(SystemState::Idle);
		SystemState desiredSystemState = getDesiredSystemState();
		if(desiredSystemState!=SystemState::Idle){
			return;
		}
		_fpgaInterface.setDrive(false);
		_fpgaInterface.setDrive2(false);
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
	setActualSystemState(SystemState::Live);
	while(true){
		if(getDesiredSystemState()!=SystemState::Live){
			break;
		}
		if(isShuttingDownOrShutDown()){
			break;
		}
		if(getInError()){
			break;
		}
		_fpgaInterface.setDrive(true);
		_fpgaInterface.setDrive2(true);
	}
	_fpgaInterface.setDrive(false);
	_fpgaInterface.setDrive2(false);
}
void HighSpeedCore::doError(){
	setActualSystemState(SystemState::Error);
	while(true){
		_fpgaInterface.setDrive(false);
		_fpgaInterface.setDrive2(false);
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
void HighSpeedCore::dispatchWarning(std::string message){
	onWarning.dispatch(message);
}
void HighSpeedCore::calculateAdditionalShutdownTime(float voltage, float& timeSeconds, float& time2Seconds){
	
	timeSeconds =   static_cast<float>(_hvpsConfiguration1.villardCapacitorsBleedTimeConstantSeconds)
					* std::log(voltage / SAFE_OUTPUT_VOLTAGE);
	time2Seconds =  static_cast<float>(_hvpsConfiguration2.villardCapacitorsBleedTimeConstantSeconds)
					* std::log(voltage / SAFE_OUTPUT_VOLTAGE);
}