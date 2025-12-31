#include "ThresholdMonitor.hpp"
#include "IO/Outputs.hpp"
#include "ADC/ADC.hpp"
#include "Storage/Flash.hpp"
#include <functional>
#include "Macros/GetFileName.hpp"
const char* ThresholdMonitor::getTag() {return GET_FILE_NAME;}

ThresholdMonitor::ThresholdMonitor(
	adc_channel_t ch, 
	const VoltageFeedbackModuleConfiguration& config1, const VoltageFeedbackModuleConfiguration& config2
):
_config1(config1),
_config2(config2),
_monitorVoltageThresholdHandle(nullptr),
_forceReached(false),
_actualReached(false)
{
	float initialUnscaledVoltageThreshold = _config1.defaultThreshold;
	Flash::getFloat(FLASH_NAMESPACE, THRESHOLD_VOLTAGE_KEY, initialUnscaledVoltageThreshold);
	float scaledThreshold = toScaledADCThreshold(initialUnscaledVoltageThreshold);
	//Inputs::selectADCVoltageDividerInputAsChannel();
	_monitorVoltageThresholdHandle = 
		ADC::monitorVoltageThresholdWithNewPriorityTask(
			ch,
			scaledThreshold,
			[this](bool reached) { onVoltageThresholdReachedChanged(reached); }
		);
}
float ThresholdMonitor::getVoltage(){
	return _monitorVoltageThresholdHandle->getVoltage();
}
void ThresholdMonitor::setThresholdVoltage(float voltage){
	_monitorVoltageThresholdHandle->setThresholdVoltage(voltage);
	Flash::setFloat(FLASH_NAMESPACE, THRESHOLD_VOLTAGE_KEY, voltage);
}
void ThresholdMonitor::setForce(bool value) noexcept{
	LOG_INFO("setForce called");
	_forceReached.store(value, std::memory_order_relaxed);
	if(value){
		LOG_INFO("forcing to reached");
		Outputs::setThresholdReached(true);
		return;
	}
	LOG_INFO("forcing to not reached");
	bool actual = _actualReached.load(std::memory_order_relaxed);
	LOG_INFO(actual?"actual was true":"actual was false");
	Outputs::setThresholdReached(actual);
}
void ThresholdMonitor::onVoltageThresholdReachedChanged(bool reached)noexcept{
	LOG_INFO("ACTUAL SET from ThresholdMonitor::onVoltageThresholdReachedChanged");
	LOG_INFO(reached?"to true":"to false");
	_actualReached.store(reached, std::memory_order_relaxed);
	Outputs::setThresholdReached(
		reached||_forceReached.load(std::memory_order_relaxed));
}
float ThresholdMonitor::toScaledADCThreshold(float vUnscaled)noexcept{
	return vUnscaled/_config1.vHvOverVadcRatio;
}