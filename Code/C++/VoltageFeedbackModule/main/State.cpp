#include "State.hpp"
#include "Logging/Log.hpp"
#include "System/Aborter.hpp"
#include "Storage/Flash.hpp"
const char* State::TAG = "State";
const char* State::FLASH_NAMESPACE = "s";
const char* State::THRESHOLD_VOLTAGE = "tv";
State* State::_instance = nullptr;
State& State::initialize(MonitorVoltageThresholdHandle* monitorVoltageThresholdHandle){
    if(_instance != nullptr){
        Aborter::safeAbort(TAG, "Already initialized");
        return *_instance;
    }
    _instance = new State(monitorVoltageThresholdHandle);
	double initialThresholdVoltage;
	if(Flash::getDouble(
			FLASH_NAMESPACE, THRESHOLD_VOLTAGE, initialThresholdVoltage)){
		Log::Info(TAG, "Setting initial threshold voltage to %f from memory", initialThresholdVoltage);
		_instance->_monitorVoltageThresholdHandle->setThresholdVoltage(initialThresholdVoltage);
	}
	else{
		Log::Info(TAG, "Had no initial thresholdVoltage");
	}
    return *_instance;
}
State::State(MonitorVoltageThresholdHandle* monitorVoltageThresholdHandle)
:_monitorVoltageThresholdHandle(monitorVoltageThresholdHandle){
	
}
void State::setVoltageThreshold(double value){
	_monitorVoltageThresholdHandle->setThresholdVoltage(value);
	Flash::setDouble(FLASH_NAMESPACE, THRESHOLD_VOLTAGE, value);
	
}

