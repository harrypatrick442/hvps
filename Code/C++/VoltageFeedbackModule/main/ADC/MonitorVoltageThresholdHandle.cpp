#include "MonitorVoltageThresholdHandle.hpp"
MonitorVoltageThresholdHandle::MonitorVoltageThresholdHandle(
	double initialVoltage, ReverseVoltageToRawLookup* reverseLookup):exit(false){
	_reverseLookup = reverseLookup;
	this->setThresholdVoltage(initialVoltage);
}
void MonitorVoltageThresholdHandle::setThresholdVoltage(double voltage){
	rawThreshold = _reverseLookup->lookupVolts(voltage);
}
void MonitorVoltageThresholdHandle::stop(){
	exit = true;
}