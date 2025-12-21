#include "LiveDataBroadcaster.hpp"
#include "Generated/Messages/LiveDataMessage.hpp"
#include "Macros/GetFileName.hpp"
const char* LiveDataBroadcaster::getTag() {return GET_FILE_NAME;}
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
		_liveDataCache.getFirstStageVoltage().f, 
		_liveDataCache.getOutputCurrent().f, 
		_liveDataCache.getOutputVoltage().f, 
		_liveDataCache.getPeakPrimaryCurrent().f, 
		_liveDataCache.getTotalOutputEnergy().f);
	_portControllingMachine.sendLiveData(liveDataMessage);
}