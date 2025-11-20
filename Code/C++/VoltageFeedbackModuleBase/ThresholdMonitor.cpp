#include "ThresholdMonitor.hpp"
#include "IO/Outputs.hpp"
#include "ADC/ADC.hpp"
#include "Storage/Flash.hpp"
#include <functional>

ThresholdMonitor::ThresholdMonitor(
	adc_channel_t ch, 
	const Configuration& config1, const Configuration& config2
):
_config1(config1),
_config2(config2),
_monitorVoltageThresholdHandle(nullptr)
{
	double initialUnscaledVoltageThreshold = _config1.defaultThreshold;
	Flash::getDouble(FLASH_NAMESPACE, THRESHOLD_VOLTAGE_KEY, initialUnscaledVoltageThreshold);
	double scaledThreshold = toScaledADCThreshold(initialUnscaledVoltageThreshold);
	//Inputs::selectADCVoltageDividerInputAsChannel();
	_monitorVoltageThresholdHandle = 
		ADC::monitorVoltageThresholdWithNewPriorityTask(
			ch,
			scaledThreshold,
			[this](bool reached) { onVoltageThresholdReachedChanged(reached); }
		);
}
double ThresholdMonitor::getVoltage(){
	return _monitorVoltageThresholdHandle->getVoltage();
}
void ThresholdMonitor::setThresholdVoltage(double voltage){
	_monitorVoltageThresholdHandle->setThresholdVoltage(voltage);
	Flash::setDouble(FLASH_NAMESPACE, THRESHOLD_VOLTAGE_KEY, voltage);
}
void ThresholdMonitor::onVoltageThresholdReachedChanged(bool reached)noexcept{
	Outputs::setThresholdReachedFiberOpticOnOff(reached);
}
double ThresholdMonitor::toScaledADCThreshold(double vUnscaled)noexcept{
	return vUnscaled/_config1.vHvOverVadcRatio;
}