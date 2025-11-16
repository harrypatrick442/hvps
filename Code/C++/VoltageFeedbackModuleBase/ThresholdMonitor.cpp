#include "ThresholdMonitor.hpp"
#include "IO/Outputs.hpp"
#include "ADC/ADC.hpp"
#include "NonVolatileState.hpp"
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
	NonVolatileState::getVoltageThreshold(initialUnscaledVoltageThreshold);
	double scaledThreshold = toScaledADCThreshold(initialUnscaledVoltageThreshold);
	//Inputs::selectADCVoltageDividerInputAsChannel();
	_monitorVoltageThresholdHandle = 
		ADC::monitorVoltageThresholdWithNewPriorityTask(
			ch,
			scaledThreshold,
			[this](bool reached) { onVoltageThresholdReachedChanged(reached); }
		);
	
	_eventConnectionVoltageThresholdChanged = NonVolatileState::onVoltageThresholdChanged.addHandler(
		[this](double vUnscaled){
			_monitorVoltageThresholdHandle->setThresholdVoltage(toScaledADCThreshold(vUnscaled));
		}
	);
}
double ThresholdMonitor::getVoltage(){
	return _monitorVoltageThresholdHandle->getVoltage();
}
void ThresholdMonitor::setThresholdVoltage(double voltage){
	_monitorVoltageThresholdHandle->setThresholdVoltage(voltage);
}
void ThresholdMonitor::onVoltageThresholdReachedChanged(bool reached)noexcept{
	Outputs::setThresholdReachedFiberOpticOnOff(reached);
}
double ThresholdMonitor::toScaledADCThreshold(double vUnscaled)noexcept{
	return vUnscaled/_config1.vHvOverVadcRatio;
}