#include "CurrentAndPowerThresholdMonitor.hpp"
#include "IO/Outputs.hpp"
#include "ADC/ADC.hpp"
#include <functional>

CurrentAndPowerThresholdMonitor::CurrentAndPowerThresholdMonitor(
	const Configuration& config1, const Configuration& config2
):
_config1(config1),
_config2(config2),
_monitorCurrentAndPowerHandle(nullptr)
{
	_monitorCurrentAndPowerHandle = ADC::monitorCurrentAndPower(
		senseResistanceOhms = _config1.senseResistanceOhms, 
		outputCurrentLimitingResistanceOhms = _config1.outputCurrentLimitingResistanceOhms,
		cumulativeEnergyThresholdJ = _config1.cumulativeEnergyThresholdJ,
		energyDisipatedJPerS = _config1.energyDisipatedJPerS,
		[this](bool reachedThreshold) { 
			onThresholdReachedChanged(reachedThreshold); 
		}
	);
}
void CurrentAndPowerThresholdMonitor::validateConfigs(Configuration& config1, Configuration& config2){
	if(_config1.senseResistanceOhms != _config2.senseResistanceOhms){
		Aborter::safeAbort(TAG, "senseResistanceOhms corrupted");
		return;
	}
	if(_config1.outputCurrentLimitingResistanceOhms != _config2.outputCurrentLimitingResistanceOhms){
		Aborter::safeAbort(TAG, "outputCurrentLimitingResistanceOhms corrupted");
		return;
	}
	if(_config1.cumulativeEnergyThresholdJ != _config2.cumulativeEnergyThresholdJ){
		Aborter::safeAbort(TAG, "cumulativeEnergyThresholdJ corrupted");
		return;
	}
	if(_config1.energyDisipatedJPerS != _config2.energyDisipatedJPerS){
		Aborter::safeAbort(TAG, "energyDisipatedJPerS corrupted");
		return;
	}
}
void CurrentAndPowerThresholdMonitor::onThresholdReachedChanged(bool reached)noexcept{
	Outputs::setThresholdReachedFiberOpticOnOff(reached);
}