#include "PowerConditioningMonitor.hpp"
#include "../IO/Inputs.hpp"
#include "../IO/Outputs.hpp"
#include "ADC/ADC.hpp"
#include "Core/ArrayHelper.hpp"
#include "Logging/Log.hpp"
#include "System/SafeAbort.hpp"
#include "Timing/Delay.hpp"
#include "Timing/TimeHelper.hpp"
#include "Macros/GetFileName.hpp"
#include "Tasks/TaskFactory.hpp"
const char* PowerConditioningMonitor::getTag() {return GET_FILE_NAME;}
PowerConditioningMonitor::PowerConditioningMonitor(
	const HVPSConfiguration& config1, const HVPSConfiguration& config2):
_config1(config1),
_config2(config2),
_index(0),
_talkNext(0),
_ready(false){
	TaskFactory::createNonPriorityTask([this](){
		run();
	}, "PowerConditioningMonitor::run");
}
void PowerConditioningMonitor::run(){
	Outputs::setSoftStartResistorBypassOnOff(false);
	if(_config1.vPsOverVadcRatio!=_config2.vPsOverVadcRatio){
		SAFE_ABORT("A value for the potential divider ratio corrupted");
		return;
	}
	float _lastAverageVoltage=0.0f;
	Inputs::useADCPowerSupplyVoltageFeedbackChannel([&](IADCSession&& adc){
		while(true){
			float averageVoltage = computeLatestAverageVoltage(std::forward<IADCSession>(adc));
			//LOG_INFO("averageVoltage %f", averageVoltage);
			float lastAverageVoltage = _lastAverageVoltage;
			_lastAverageVoltage = averageVoltage;
			
			if(getIsReady()){
				if(averageVoltage <= READY_VOLTAGE_HYSTERESIS_WINDOW_LOWER_BOUND){
					updateReady(false);
					LOG_INFO("Reached minimum voltage and voltage of %f V and stopped increasing", lastAverageVoltage);
				}
				continue;
			}
			
			bool reachedMinimumVoltage = averageVoltage>=READY_VOLTAGE_HYSTERESIS_WINDOW_UPPER_BOUND;
			bool voltageStoppedIncreasing = (averageVoltage - lastAverageVoltage)<=0.0f;
			if(reachedMinimumVoltage&&voltageStoppedIncreasing){
				Delay::ms(RELAY_SWITCH_TIME_MILLISECONDS);
				updateReady(true);
				LOG_INFO("Reached minimum voltage and voltage of %f V and stopped increasing", lastAverageVoltage);
				continue;
			}
			if(canTalk()){
				LOG_INFO(tellWhatWaitingOn(reachedMinimumVoltage, voltageStoppedIncreasing));
			}
		}
	});
}
void PowerConditioningMonitor::waitForSoftStart(){
	while(true){
		Delay::ms(100);
		if(getIsReady()){
			Delay::ms(10000);
			return;
		}
	}
}
bool PowerConditioningMonitor::getIsReady(){
	return _ready.load(std::memory_order_relaxed);
}
void PowerConditioningMonitor::updateReady(bool value){
	bool previous = _ready.exchange(value);
	if(previous==value)return;
	Outputs::setSoftStartResistorBypassOnOff(value);
}
bool PowerConditioningMonitor::canTalk(){	
	uint64_t now = TimeHelper::ms();
	if(now>_talkNext){
		_talkNext = now+TALK_INTERVAL_MILLISECONDS;
		return true;
	}
	return false;
}
const char* PowerConditioningMonitor::tellWhatWaitingOn(bool reachedMinimumVoltage, bool voltageStoppedIncreasing) {
	if (reachedMinimumVoltage) {
		if (voltageStoppedIncreasing)
			return "Prohibited state";//Not reachable
		return "Waiting for voltage to stabilize...";
	}
	if (voltageStoppedIncreasing)
		return "Voltage has not reached minimum but has stopped increasing...";
	return "Waiting for voltage to reach minimum threshold and stop increasing...";
}
float PowerConditioningMonitor::computeLatestAverageVoltage(IADCSession&& adc){
	_index = 0;
	while(_index<WINDOW_SAMPLES){
		_voltages[_index++] = adc.getVoltage()*_config1.vPsOverVadcRatio;
		Delay::ms(SAMPLE_INTERVAL_MS);
	}
	return ArrayHelper::average(_voltages, WINDOW_SAMPLES);
}