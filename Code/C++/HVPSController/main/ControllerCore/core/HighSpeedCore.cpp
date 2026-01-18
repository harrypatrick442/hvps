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
	HVPSConfiguration& config1,
	HVPSConfiguration& config2,
	Port_FirstStageVoltageFeedback& portFirstStageVoltageFeedback, 
	Port_OutputVoltageFeedback& portOutputVoltageFeedback,
	LiveDataCache& liveDataCache
):
_config1(config1),
_config2(config2),
_portFirstStageVoltageFeedback(portFirstStageVoltageFeedback),
_portOutputVoltageFeedback(portOutputVoltageFeedback),
_liveDataCache(liveDataCache),
_shuttingOrShutDown(false),
_actualSystemState(SystemState::Idle),
_desiredSystemState(SystemState::Idle),
_shuttingOrShutDown_2(false),
_systemChecksResult(nullptr),
_runSystemChecksLatch(),
_watchdogFed(true),
_watchdogFail(false){
	
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
	dispatchError(result->getErrorMessage());
    return result; // refcount is incremented, safe after unlock
}
void HighSpeedCore::shutDown(){
	setDesiredSystemState(SystemState::ShutDown);
	_shuttingOrShutDown.store(true, std::memory_order_relaxed);
	_shuttingOrShutDown_2.store(true, std::memory_order_relaxed);
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
void HighSpeedCore::setActualSystemState(SystemState systemState){
	_actualSystemState.store(systemState, std::memory_order_relaxed);
	dispatchSystemStateChanged(systemState);
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
	TaskFactory::createPriorityTask(_run_taskTrampoline, this, "HighSpeedCore::_run");
}
void  HighSpeedCore::_run_taskTrampoline(void* arg) {
    static_cast<HighSpeedCore*>(arg)->_run();
}
void HighSpeedCore::_run(){
	
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
				SAFE_ABORT("Illegal state");
				return;
				
		}
	}
}
std::shared_ptr<SystemChecksResult> HighSpeedCore::doSystemChecks(){
	;
	std::shared_ptr<SystemChecksResult> result = SystemChecks::run();
	std::unique_lock<std::mutex> lock(_mutexSystemChecksResult);
	_systemChecksResult = result;
	return result;
}
void HighSpeedCore::doShutDown(){
	bool v = true;
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
		FloatAndTime outputVoltageAndTime = _liveDataCache.getOutputVoltage();
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
	if(!systemChecksResult->getSuccess()){
		dispatchError(systemChecksResult->getErrorMessage());
		return;
	}
	uint64_t time, endTime, endTime_2;
	setActualSystemState(SystemState::Live);
    std::optional<InterruptTimer> liveWatchdog;
	feedWatchdog();
    liveWatchdog = initializeLiveWatchdog();
    if (!liveWatchdog.has_value()) {
        return;
    }
	_watchdogFail = false;
	liveWatchdog->getPeriodUs();
	feedWatchdog();
	endTime = TimeHelper::us()+(liveWatchdog->getPeriodUs()+10);
	while (TimeHelper::us() < endTime) {}
	if(!_watchdogFail){
		SAFE_ABORT("Watchdog test failed");
		return;
	}
	feedWatchdog();
	_watchdogFail = false;
	while(true){		
		if(_watchdogFail){
			SAFE_ABORT("Watchdog tripped");
			return;
		}
		endTime = TimeHelper::us()+_config1.onTimeMicroSeconds;
		endTime_2 = TimeHelper::us()+_config2.onTimeMicroSeconds;
		if(isShuttingDownOrShutDown()){
			break;
		}
		feedWatchdog();
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
		feedWatchdog();
		
		endTime = TimeHelper::us()+_config1.offTimeMicroSeconds;
		endTime_2 = TimeHelper::us()+_config2.offTimeMicroSeconds;
		
		if(getDesiredSystemState()!=SystemState::Live){
			break;
		}
		while(true){
			feedWatchdog();
			time = TimeHelper::us();
			if(time>=endTime){
				break;
			}
			if(time>=endTime_2){
				break;
			}
		}
	}
	feedWatchdog();
}
void HighSpeedCore::dispatchSystemStateChanged(SystemState v){
	onSystemStateChanged.dispatch(v);
}
void HighSpeedCore::dispatchError(std::string errorMessage){
	onError.dispatch(errorMessage);
}
std::optional<InterruptTimer> HighSpeedCore::initializeLiveWatchdog()
{
    const uint32_t periodUs_1 = _config1.onTimeMicroSeconds * 2;
    const uint32_t periodUs_2 = _config2.onTimeMicroSeconds * 2;

    if (periodUs_1 != periodUs_2) {
        SAFE_ABORT("Memory corruption");
    }

    InterruptTimer timer(
        TIMER_GROUP_0,
        TIMER_0,
        periodUs_1,
        ESP_INTR_FLAG_LEVEL3,
        true
    );

    if (periodUs_1 != periodUs_2) {
        SAFE_ABORT("Stack corruption during watchdog init");
    }

    esp_err_t err = timer.configure(&HighSpeedCore::liveWatchdogTimerTrampoline, this);
    if (err != ESP_OK) {
        SAFE_ABORT("Failed to initialize timer interrupt");
        return std::nullopt;
    }

    return timer; // move-constructed into optional
}

inline void IRAM_ATTR HighSpeedCore::feedWatchdog()
{
	_watchdogFed = true;
}
inline void IRAM_ATTR HighSpeedCore::checkWatchdog()
{
	if(_watchdogFed){
		_watchdogFed = false;
		return;
	}
	Outputs::setMOSFETOnOff(false);
	_watchdogFail = true;
}

bool IRAM_ATTR HighSpeedCore::liveWatchdogTimerTrampoline(void *arg)
{
    static_cast<HighSpeedCore*>(arg)->checkWatchdog();
    return false;  // no context switch needed
}