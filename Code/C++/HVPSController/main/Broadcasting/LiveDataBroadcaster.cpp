#include "LiveDataBroadcaster.hpp"
#include "Generated/Messages/LiveDataMessage.hpp"
#include "Macros/GetFileName.hpp"
const char* LiveDataBroadcaster::getTag() {return GET_FILE_NAME;}
LiveDataBroadcaster::LiveDataBroadcaster(
	LiveDataCache& liveDataCache, 
	Port_ControllingMachine& portControllingMachine,
	FrequencyMeter& frequencyMeter
):
	_liveDataCache(liveDataCache),
	_portControllingMachine(portControllingMachine),
	_frequencyMeter(frequencyMeter),
	_timer(500, 
		[this](){
			this->_run();
		}, true
	){
	_eventConnectionPortOnOpen = _portControllingMachine.onOpened.addHandler(
		[this](){
			this->start();
		}
	);
	_eventConnectionPortOnClose = _portControllingMachine.onClosed.addHandler(
		[this](){
			this->stop();
		}
	);
	if(_portControllingMachine.getIsOpen()){
		this->start();
	}
}
void LiveDataBroadcaster::start(){
	_timer.start();
}
void LiveDataBroadcaster::stop(){
	_timer.stop();
}
void LiveDataBroadcaster::_run(){
	uint64_t frequencyHz;
	std::optional<uint64_t> frequencyOptional;
	if (_frequencyMeter.calculateAndRestart(frequencyHz)) {
		frequencyOptional = frequencyHz;
	}
	VoltageWithRawAndTime outputVoltage = _liveDataCache.getOutputVoltage();
	VoltageWithRawAndTime firstStageVoltage = _liveDataCache.getFirstStageVoltage();
	LiveDataMessage liveDataMessage(
		firstStageVoltage.voltage,
		frequencyOptional,
		_liveDataCache.getOutputCurrent().f, 
		outputVoltage.voltage, 
		_liveDataCache.getPeakPrimaryCurrent().f, 
		_liveDataCache.getTotalOutputEnergy().f);
	_portControllingMachine.sendLiveData(liveDataMessage);
}