#include "NonVolatileState.hpp"
#include "Storage/Flash.hpp"
Event<double> NonVolatileState::onVoltageThresholdChanged;
bool NonVolatileState::getVoltageThreshold(double& value){
	return Flash::getDouble(FLASH_NAMESPACE, THRESHOLD_VOLTAGE, value);
}
void NonVolatileState::setVoltageThreshold(double value){
	Flash::setDouble(FLASH_NAMESPACE, THRESHOLD_VOLTAGE, value);
	onVoltageThresholdChanged.dispatch(value);
}

