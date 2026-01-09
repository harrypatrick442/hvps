#include "VoltageBroadcaster.hpp"
#include "Macros/GetFileName.hpp"
#include "Logging/Log.hpp"
const char* VoltageBroadcaster::getTag() {return GET_FILE_NAME;}
VoltageBroadcaster::VoltageBroadcaster(
	IVoltageSource& voltageSource,
	Port_FiberOpticChannel1& port
):
	_voltageSource(voltageSource),
	_port(port),
	_timer(500, 
		[this](){
			this->_run();
		}, true
	){
	_timer.start();
}
void VoltageBroadcaster::_run(){
	uint16_t rawVoltage;
	float voltage = _voltageSource.getVoltage(rawVoltage);
	_port.sendVoltage(voltage, rawVoltage);
}