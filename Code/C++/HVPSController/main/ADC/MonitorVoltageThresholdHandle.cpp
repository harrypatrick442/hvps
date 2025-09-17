#include "MonitorVoltageThresholdHandle.hpp"
MonitorVoltageThresholdHandle::MonitorVoltageThresholdHandle(
	double initialVoltage, ReverseVoltageToRawLookup* reverseLookup,
    std::function<void(bool)> cb)
	:exit(false),
	callback(std::move(cb)){
	_reverseLookup = reverseLookup;
	this->setThresholdVoltage(initialVoltage);
}
void MonitorVoltageThresholdHandle::setThresholdVoltage(double voltage){
	rawThreshold = _reverseLookup->lookupVolts(voltage);
}
void MonitorVoltageThresholdHandle::stop(){
    exit.store(true, std::memory_order_relaxed);
}