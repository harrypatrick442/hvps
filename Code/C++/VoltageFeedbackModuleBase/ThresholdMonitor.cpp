#include "ThresholdMonitor.hpp"
#include "IO/Outputs.hpp"
#include "ADC/ADC.hpp"
#include "Storage/Flash.hpp"
#include <functional>

ThresholdMonitor::ThresholdMonitor(
	adc_channel_t ch, 
	const VoltageFeedbackModuleConfiguration& config1, const VoltageFeedbackModuleConfiguration& config2
):
_config1(config1),
_config2(config2),
_monitorVoltageThresholdHandle(nullptr)
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
void ThresholdMonitor::onVoltageThresholdReachedChanged(bool reached)noexcept{
	Outputs::setThresholdReachedFiberOpticOnOff(reached);
}
float ThresholdMonitor::toScaledADCThreshold(float vUnscaled)noexcept{
	return vUnscaled/_config1.vHvOverVadcRatio;
}