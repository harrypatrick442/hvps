#include "ThresholdMonitor.hpp"
#include "IO/Outputs.hpp"
#include "ADC/ADC.hpp"
#include "Timing/Delay.hpp"
#include "Tasks/TaskFactory.hpp"
#include "Storage/Flash.hpp"
#include "System/SafeAbort.hpp"
#include "Logging/Log.hpp"
#include <functional>
#include "Macros/GetFileName.hpp"
const char* ThresholdMonitor::getTag() {return GET_FILE_NAME;}
const char* ThresholdMonitor::SET_TAP_VOLTAGE_THRESHOLD_ERROR_MESSAGE = "Tap threshold voltage calculation error";
ThresholdMonitor::ThresholdMonitor(
	adc_channel_t ch, 
	const VoltageFeedbackModuleConfiguration& config1, const VoltageFeedbackModuleConfiguration& config2
):
_config1(config1),
_config2(config2),
_monitorVoltageThresholdHandle(nullptr),
_forceReached(std::nullopt),
_actualReached(false)
{
	float initialUnscaledVoltageThreshold = _config1.defaultThreshold;
	Flash::getFloat(FLASH_NAMESPACE, THRESHOLD_VOLTAGE_KEY, initialUnscaledVoltageThreshold);
	float tapThresholdVoltage, tapThresholdVoltageCopy;
	toScaledADCThreshold(initialUnscaledVoltageThreshold, tapThresholdVoltage, tapThresholdVoltageCopy);
	//Inputs::selectADCVoltageDividerInputAsChannel();
	_monitorVoltageThresholdHandle = 
		ADC::monitorVoltageThresholdWithNewPriorityTask(
			ch,
			tapThresholdVoltage,
			[this](bool reached) { onVoltageThresholdReachedChanged(reached); }
		);
	if(tapThresholdVoltage!=tapThresholdVoltageCopy){
		SAFE_ABORT(SET_TAP_VOLTAGE_THRESHOLD_ERROR_MESSAGE);
		return;
	}
	//LOG_INFO("Set tap threshold voltage to %f", tapThresholdVoltage);
	
	/*TaskFactory::createNonPriorityTask([this](){
		while(true){
			Delay::ms(1000);
			if(_forceReached){
				LOG_INFO("forcing");
			}
			else{
				LOG_INFO("not forcing");
			}
			LOG_INFO(_actualReached?"reached":"not reached");
		}
	}, "HVPSCircuitEmulator::debug");*/
}
float ThresholdMonitor::getVoltage(uint16_t& raw){
	float voltage =  fromScaledADCToActual(_monitorVoltageThresholdHandle->getVoltage(raw));
	return voltage;
}
void ThresholdMonitor::setThresholdVoltage(float voltage){
	float tapThresholdVoltage, tapThresholdVoltageCopy;
	toScaledADCThreshold(voltage, tapThresholdVoltage, tapThresholdVoltageCopy);
	_monitorVoltageThresholdHandle->setThresholdVoltage(tapThresholdVoltage);
	if(tapThresholdVoltage!=tapThresholdVoltageCopy){
		SAFE_ABORT(SET_TAP_VOLTAGE_THRESHOLD_ERROR_MESSAGE);
		return;
	}
	Flash::setFloat(FLASH_NAMESPACE, THRESHOLD_VOLTAGE_KEY, voltage);
}
void ThresholdMonitor::setForce(std::optional<bool> forceReached) noexcept{
	
	_lock.lock();
	_forceReached = forceReached;
	if(!forceReached.has_value()){
		bool actualReached = _actualReached;
		Outputs::setThresholdReached(actualReached);
		_lock.unlock();
		LOG_INFO(actualReached?"set threshold reached to true using actual":"set threshold reached to false using actual");
		return;
	}
	bool value = forceReached.value();
	Outputs::setThresholdReached(value);//TODO should lock wrap call to set actual pin
	_lock.unlock();
	LOG_INFO(value?"set threshold reached to true using force":"set threshold reached to false using force");
}
void ThresholdMonitor::onVoltageThresholdReachedChanged(bool actualReached)noexcept{
	
	_lock.lock();
	_actualReached = actualReached;
	std::optional<bool> forceReached = _forceReached;
	if(!forceReached.has_value()){
		Outputs::setThresholdReached(actualReached);
		_lock.unlock();
		//LOG_INFO(" AA not forcing");
		//LOG_INFO(actualReached?" AA reached":" AA not reached");
		LOG_INFO(actualReached?"set threshold reached to true using actual in changed":"set threshold reached to false using actual in changed");
		return;
	}
	bool value = forceReached.value();
	Outputs::setThresholdReached(value);
	_lock.unlock();
		LOG_INFO(value?"set threshold reached to true using force in changed":"set threshold reached to false using force in changed");
	//LOG_INFO(" AA forcing");
	//LOG_INFO(value?" AA forcing actualReached":" AA forcing not reached");
}
void ThresholdMonitor::toScaledADCThreshold(float vUnscaled, float& a, float& b)noexcept{
	a =  vUnscaled/_config1.vHvOverVadcRatio;
	b = vUnscaled / _config2.vHvOverVadcRatio;
	if(a!=b){
		SAFE_ABORT(SET_TAP_VOLTAGE_THRESHOLD_ERROR_MESSAGE);
	}
}
float ThresholdMonitor::fromScaledADCToActual(float adc){
	return adc * _config2.vHvOverVadcRatio;
}