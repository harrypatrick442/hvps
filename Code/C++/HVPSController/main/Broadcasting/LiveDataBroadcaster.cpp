#include "LiveDataBroadcaster.hpp"
#include "../Generated/Messages/HVPSLiveDataMessage.hpp"
const char* LiveDataBroadcaster::TAG = "LiveDataBroadcaster";
LiveDataBroadcaster::LiveDataBroadcaster(LiveDataCache& liveDataCache, Port_ControllingMachine& port_ControllingMachine):
	_liveDataCache(liveDataCache);
	_port_ControllingMachine(port_ControllingMachine),
	_timer(500, _run, true){
}
void LiveDataBroadcaster::start(){
	_timer.start();
}
void LiveDataBroadcaster::stop(){
	_timer.stop();
}
void LiveDataBroadcaster::_run(){
	HVPSLiveDataMessage hvpsLiveDataMessage(
		_liveDataCache.getFirstStageVoltage().value, 
		_liveDataCache.getOutputCurrent().value, 
		_liveDataCache.getOutputVoltage().value, 
		_liveDataCache.getPeakPrimaryCurrent().value, 
		_liveDataCache.getTotalOutputEnergy().value));
	_portControllingMachine.sendHVPSLiveData(hvpsLiveDataMessage);
}