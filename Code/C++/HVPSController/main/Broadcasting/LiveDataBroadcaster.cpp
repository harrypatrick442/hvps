#include "LiveDataBroadcaster.hpp"
#include "Generated/Messages/LiveDataMessage.hpp"
LiveDataBroadcaster::LiveDataBroadcaster(
	LiveDataCache& liveDataCache, 
	Port_ControllingMachine& portControllingMachine
):
	_liveDataCache(liveDataCache),
	_portControllingMachine(portControllingMachine),
	_timer(500, 
		[this](){
			this->_run();
		}, true
	){
}
void LiveDataBroadcaster::start(){
	_timer.start();
}
void LiveDataBroadcaster::stop(){
	_timer.stop();
}
void LiveDataBroadcaster::_run(){
	LiveDataMessage liveDataMessage(
		_liveDataCache.getFirstStageVoltage().d, 
		_liveDataCache.getOutputCurrent().d, 
		_liveDataCache.getOutputVoltage().d, 
		_liveDataCache.getPeakPrimaryCurrent().d, 
		_liveDataCache.getTotalOutputEnergy().d);
	_portControllingMachine.sendLiveData(liveDataMessage);
}